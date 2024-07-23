#include "index.h"

///// PROCESSER ///// 
void Process::processFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::size_t cnt_words = 0;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::size_t lineNumber = 1;

    std::unordered_map<std::size_t, std::shared_ptr<std::list<std::size_t>>> information;
    while (std::getline(file, line)) {
        std::string word;
        std::istringstream lineStream(line);
        while (lineStream >> word) {
            std::transform(word.begin(), word.end(), word.begin(),[](unsigned char c){ return std::tolower(c); });
            cnt_words++;
            if (!terms_.contains(word)) {
                terms_[word] = terms_.size();
            }
            if (!information[terms_[word]]) {
                information[terms_[word]] = std::make_shared<std::list<std::size_t>>();
                dataArray_.emplace_back(terms_[word], docID, information[terms_[word]]);
            }
            information[terms_[word]]->push_back(lineNumber);
        }
        lineNumber++;
    }

    documents_.emplace_back(cnt_words, dirIterator_->path().lexically_relative(directoryPath_));
}

///// PROCESSER ///// 
void Process::buildIndex() {
    for (; dirIterator_ != std::filesystem::end(dirIterator_); ++dirIterator_) {
        if (!std::filesystem::is_directory(*dirIterator_)) {
            processFile(dirIterator_->path().string());
            docID++;
        }
        if (dataArray_.size() == maxCountRecords) {
            std::sort(dataArray_.begin(), dataArray_.end(), [](const Data& a, const Data& b) {if (a.first == b.first) {return a.second < b.second;} return a.first < b.first;});
            index_.uploadDataToDisk(dataArray_);
            index_.uploadDocumentsToDisk(documents_);
            this->clearTables();
        }
    }
    std::sort(dataArray_.begin(), dataArray_.end(), [](const Data& a, const Data& b) {if (a.first == b.first) {return a.second < b.second;} return a.first < b.first;});
    index_.uploadDataToDisk(dataArray_);
    index_.uploadDocumentsToDisk(documents_);
    index_.uploadTermsToDisk(terms_);
    this->clearTables();
    index_.mergeBlocks();
}

///// PROCESSER ///// 
bool Process::isFinished() {
    if (dirIterator_ == std::filesystem::end(dirIterator_)) {
        return true;
    }

    return false;
}

///// PROCESSER ///// 
void Process::clearTables() {
    dataArray_.clear();
    documents_.clear();
}

///// INDEX /////
void Index::uploadDataToDisk(const std::vector<Data>& dataArray) {
    std::filesystem::path path = indexPath_;
    path /= "db" + std::to_string(number_block) + ".bin";
    number_block++;

    std::ofstream outFile(path, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open the file for writing: " << path << std::endl;
        return;
    }

    std::size_t dataCount = dataArray.size();
    outFile.write(reinterpret_cast<const char*>(&dataCount), 8);
    for (const auto& entry : dataArray) {
        outFile.write(reinterpret_cast<const char*>(&entry.first), 8);
        outFile.write(reinterpret_cast<const char*>(&entry.second), 8);
        if (entry.third) {
            std::size_t size = entry.third->size();
            outFile.write(reinterpret_cast<const char*>(&size), 8);
            for (std::size_t value : *entry.third) {
                outFile.write(reinterpret_cast<const char*>(&value), 8);
            }
        }
    }

    outFile.close();
}

///// INDEX /////
void Index::uploadDocumentsToDisk(const std::vector<documentID>& documents) {
    std::size_t docCount = documents.size();
    std::vector<std::size_t> offsets(docCount);
    offsets[0] = 0;
    for (std::size_t i = 1; i < docCount; i++) {
        offsets[i] = offsets[i - 1] + 8 + documents[i - 1].second.string().size() + 8;
    }

    std::filesystem::path path = indexPath_;
    path /= "docs" + std::to_string(numberDoc) + ".bin";
    numberDoc++;

    std::ofstream outFile(path, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open the file for writing: " << path << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&docCount), 8);
    
    for (const auto& offset : offsets) {
        outFile.write(reinterpret_cast<const char*>(&offset), 8);
    }

    for (const auto& [id, path] : documents) {
        const std::string& pathStr = path.string();
        std::size_t pathLength = pathStr.size();

        outFile.write(reinterpret_cast<const char*>(&pathLength), 8);
        outFile.write(pathStr.c_str(), pathLength);
        outFile.write(reinterpret_cast<const char*>(&id), 8);
        allWords += id;
    }
    outFile.write(reinterpret_cast<const char*>(&allWords), 8);

    outFile.close();
}

///// INDEX /////
void Index::uploadTermsToDisk(const std::unordered_map<std::string, std::size_t>& terms) {
    std::filesystem::path path = indexPath_;
    path /= "terms.bin";

    std::ofstream outFile(path, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open the file for writing: " << path << std::endl;
        return;
    }

    std::size_t termCount = terms.bucket_count();
    
    std::vector<std::size_t> offsets(termCount);
    offsets[0] = 0;

    for (std::size_t i = 1; i < termCount; ++i) {
        std::size_t bucketSize = 0;
        auto it = terms.begin(i - 1);
        auto end = terms.end(i - 1);
        for (; it != end; ++it) {
            bucketSize += 8;
            bucketSize += it->first.size();
            bucketSize += 8;
        }
        if (terms.bucket_size(i - 1) > 0) {
            offsets[i] = offsets[i - 1] + bucketSize + 8;
        } else {
            offsets[i] = offsets[i - 1] + bucketSize;
        }
    }

    outFile.write(reinterpret_cast<const char*>(&termCount), 8);

    for (std::size_t offset : offsets) {
        outFile.write(reinterpret_cast<char*>(&offset), 8);
    }

    for (std::size_t i = 0; i < termCount; ++i) {
        if (terms.bucket_size(i) > 0) {
            std::size_t szBuc = terms.bucket_size(i);
            outFile.write(reinterpret_cast<const char*>(&szBuc), 8);
            auto it = terms.begin(i);
            auto end = terms.end(i);
            for (; it != end; ++it) {
                std::size_t keySize = it->first.size();
                outFile.write(reinterpret_cast<const char*>(&keySize), 8);
                outFile.write(it->first.data(), keySize);
                outFile.write(reinterpret_cast<const char*>(&it->second), 8);
            }
        }
    }

    outFile.close();
}

///// INDEX /////
void readData(Data& data, std::ifstream& file) {
    char temp[8];
    file.read(temp, 8);
    std::memcpy(&data.first, temp, 8);
    file.read(temp, 8);
    std::memcpy(&data.second, temp, 8);

    std::size_t size;
    file.read(temp, 8);
    std::memcpy(&size, temp, 8);

    data.third = std::make_shared<std::list<std::size_t>>();
    for (std::size_t j = 0; j < size; ++j) {
        std::size_t element;
        file.read(temp, 8);
        std::memcpy(&element, temp, 8);
        data.third->push_back(element);
    }
}

///// INDEX /////
void Index::mergeBlocks() {
    std::size_t sizeAllBlocks = 0;
    std::filesystem::recursive_directory_iterator Iterator(indexPath_);
    std::vector<std::ifstream> filesToMerge;
    std::vector<std::string> filesPathes;
    char temp[8];

    std::filesystem::path path = indexPath_;
    path /= "tempIndex.bin";
    std::ofstream outFile(path, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open the file for writing: " << path << std::endl;
        return;
    }

    for (auto& iter : Iterator) {
        if (iter.path().filename().string().starts_with("db")) {
            filesToMerge.emplace_back(iter.path(), std::ios::binary);
            filesPathes.push_back(iter.path().string());
            if (!filesToMerge.back()) {
                std::cerr << "Failed to open the file for writing: " << iter.path() << std::endl;
                return;
            }
        }
    }

    std::vector<Data> curData(filesToMerge.size());
    std::vector<std::size_t> sizes(filesToMerge.size());
    for (std::size_t i = 0; i < filesToMerge.size(); i++) {
        filesToMerge[i].read(temp, 8);
        std::memcpy(&sizes[i], temp, 8);
        sizeAllBlocks += sizes[i];
    }

    for (int i = 0; i < filesToMerge.size();){
        if (filesToMerge[i].eof() || filesToMerge[i].fail()) {
            filesToMerge[i].close();
            filesToMerge.erase(filesToMerge.begin() + i);
            curData.erase(curData.begin() + i);
            continue;
        }
        readData(curData[i], filesToMerge[i]);
        ++i;
    }

    while (!filesToMerge.empty()) {
        std::size_t curIdx = 0;
        std::size_t minTermId = 10000000;
        for (std::size_t i = 0; i < filesToMerge.size(); i++) {
            if (minTermId >= curData[i].first) {
                minTermId = curData[i].first;
                curIdx = i;
            }
        }
        outFile.write(reinterpret_cast<char*>(&curData[curIdx].first), 8);
        outFile.write(reinterpret_cast<char*>(&curData[curIdx].second), 8);
        std::size_t size = curData[curIdx].third->size();
        outFile.write(reinterpret_cast<const char*>(&size), 8);
        for (std::size_t value : *curData[curIdx].third) {
            outFile.write(reinterpret_cast<const char*>(&value), 8);
        }
        sizes[curIdx]--;
        
        if (sizes[curIdx] == 0) {
            filesToMerge[curIdx].close();
            std::remove(filesPathes[curIdx].c_str());
            filesToMerge.erase(filesToMerge.begin() + curIdx);
            curData.erase(curData.begin() + curIdx);
            sizes.erase(sizes.begin() + curIdx);
        } else {
            readData(curData[curIdx], filesToMerge[curIdx]);
        }
    }
    outFile.close();

///// Создание массива //////
    std::ifstream tempFile(path, std::ios::binary);
    if (!tempFile) {
        std::cerr << "Failed to open the file for writing: " << path << std::endl;
        return;
    }
    std::filesystem::path generalPath = indexPath_;
    generalPath /= "invertIndex.bin";
    std::ofstream indexFile(generalPath, std::ios::binary);
    if (!indexFile) {
        std::cerr << "Failed to open the file for writing: " << generalPath << std::endl;
        return;
    }

    indexFile.write(reinterpret_cast<char*>(&sizeAllBlocks), 8);

    std::size_t tempId = 0;
    std::size_t nextTempId = 0;
    std::size_t prevOffset = 0;
    std::vector<std::size_t> counts(sizeAllBlocks);
    
    tempFile.read(temp, 8);
    std::memcpy(&tempId, temp, 8);
    nextTempId = tempId;
    counts[nextTempId]++;
    tempFile.seekg(8, std::ios::cur);
    indexFile.write(reinterpret_cast<const char*>(&prevOffset), 8);

    for (std::size_t i = 1; i < sizeAllBlocks; ++i) {
        std::size_t offsetSize = 0;
        bool flag = true;

        while (flag && !(tempFile.eof() || tempFile.fail())) {
            std::size_t size;
            tempFile.read(temp, 8);
            std::memcpy(&size, temp, 8);
            std::size_t szList = size * 8;
            tempFile.seekg(szList, std::ios::cur);
            
            tempFile.read(temp, 8);
            std::memcpy(&nextTempId, temp, 8);
            tempFile.seekg(8, std::ios::cur);
            if (tempFile.eof() || tempFile.fail()) {
                break;
            }
            
            offsetSize = offsetSize + 16 + szList;
            counts[nextTempId]++;

            if (tempId != nextTempId) {
                offsetSize += 8;
                tempId = nextTempId;
                flag = false;
            }
        }
        prevOffset += offsetSize;
        indexFile.write(reinterpret_cast<const char*>(&prevOffset), 8);
    }

    tempFile.close();
    tempFile.open(path, std::ios::binary);

    while (sizeAllBlocks-- != 0) {
        Data tempData;
        readData(tempData, tempFile);
        if (tempData.first != tempId) {
            indexFile.write(reinterpret_cast<char*>(&counts[tempData.first]), 8);
            tempId = tempData.first;
        }
        indexFile.write(reinterpret_cast<char*>(&tempData.second), 8);
        std::size_t size = tempData.third->size();
        indexFile.write(reinterpret_cast<const char*>(&size), 8);
        for (std::size_t value : *tempData.third) {
            indexFile.write(reinterpret_cast<const char*>(&value), 8);
        }
        if (tempFile.eof()) {
            break;
        }
    }

    tempFile.close();
    indexFile.close();
    std::remove(path.string().c_str());
}

///// INDEX /////
std::string Index::getDocumentName(std::size_t idx) {
    char temp[8];
    std::filesystem::path path = indexPath_;
    path /= "docs1.bin";
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open the file for reading");
    }
    
    std::size_t sizeDoc;
    file.read(temp, 8);
    std::memcpy(&sizeDoc, temp, 8);
    std::size_t toSkip = idx * 8;
    file.seekg(toSkip, std::ios::cur);
    std::size_t pos;
    file.read(temp, 8);
    std::memcpy(&pos, temp, 8);

    std::size_t sizePath;
    file.seekg(8 + (sizeDoc * 8) + pos, std::ios::beg);
    file.read(temp, 8);
    std::memcpy(&sizePath, temp, 8);
    
    std::string pathStr;
    pathStr.resize(sizePath);
    file.read(&pathStr[0], sizePath);

    return pathStr;
}

///// INDEX /////
std::size_t Index::getDocumentSize(std::size_t idx) {
    char temp[8];
    std::filesystem::path path = indexPath_;
    path /= "docs1.bin";
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open the file for reading");
    }
    
    std::size_t sizeDoc;
    file.read(temp, 8);
    std::memcpy(&sizeDoc, temp, 8);
    std::size_t toSkip = idx * 8;
    file.seekg(toSkip, std::ios::cur);
    std::size_t pos;
    file.read(temp, 8);
    std::memcpy(&pos, temp, 8);

    std::size_t sizePath;
    file.seekg(8 + (sizeDoc * 8) + pos, std::ios::beg);
    file.read(temp, 8);
    std::memcpy(&sizePath, temp, 8);
    
    std::size_t size;
    file.seekg(sizePath, std::ios::cur);
    file.read(temp, 8);
    std::memcpy(&size, temp, 8);

    return size;
}

///// INDEX /////
std::pair<std::size_t, bool> Index::getTermId(const std::string& term) {
    char temp[8];
    std::string word = term;
    std::transform(word.begin(), word.end(), word.begin(),[](unsigned char c){ return std::tolower(c); });

    std::filesystem::path path = indexPath_;
    path /= "terms.bin";
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open the file for reading");
    }

    std::size_t cntBuckets;
    file.read(temp, 8);
    std::memcpy(&cntBuckets, temp, 8);
    std::hash<std::string> hash;
    std::size_t idx = hash(word) % cntBuckets;
    file.seekg(idx * 8, std::ios::cur);
    std::size_t pos;
    file.read(temp, 8);
    std::memcpy(&pos, temp, 8);

    std::size_t sizeKey;
    file.seekg(8 + (cntBuckets * 8) + pos, std::ios::beg);
    file.read(temp, 8);
    std::memcpy(&sizeKey, temp, 8);

    for (int i = 0; i < sizeKey; i++) {
        std::size_t szKey;
        std::string key;
        std::size_t value;

        file.read(temp, 8);
        std::memcpy(&szKey, temp, 8);
        key.resize(szKey);
        file.read(&key[0], szKey);
        file.read(temp, 8);
        std::memcpy(&value, temp, 8);
        
        if (key == word) {
            return{value, true};
        }
    }

    return {0, false};
}

std::size_t Index::getDocsCount() {
    char temp[8];
    std::filesystem::path path = indexPath_;
    path /= "docs" + std::to_string(numberDoc) + ".bin";

    std::ifstream outFile(path, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open the file for reading");
    }

    std::size_t docCount;
    outFile.read(temp, 8);
    std::memcpy(&docCount, temp, 8);
    return docCount;
}

std::size_t Index::getDocsCountToTerm(std::size_t idx) {
    char temp[8];
    std::ifstream file(pathIndexG, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open the file for reading");
    }

    std::size_t size;
    std::size_t cntDocs;
    file.read(temp, 8);
    std::memcpy(&cntDocs, temp, 8);
    file.seekg(idx * 8, std::ios::cur);
    std::size_t pos;
    file.read(temp, 8);
    std::memcpy(&pos, temp, 8);
    file.seekg(pos + 8 + cntDocs * 8, std::ios::beg);
    file.read(temp, 8);
    std::memcpy(&size, temp, 8);

    return size;
}

std::size_t Index::getPostLIstSz(std::size_t idx) {
    char temp[8];
    std::ifstream fileStream(pathIndexG, std::ios::binary);
    if (!fileStream) {
        throw std::runtime_error("Unable to open file");
    }

    std::size_t size;

    std::size_t cntDocs;
    fileStream.read(temp, 8);
    std::memcpy(&cntDocs, temp, 8);
    fileStream.seekg(idx * 8, std::ios::cur);
    std::size_t pos;
    fileStream.read(temp, 8);
    std::memcpy(&pos, temp, 8);
    fileStream.seekg(pos + 8 + cntDocs * 8, std::ios::beg);
    fileStream.read(temp, 8);
    std::memcpy(&size, temp, 8);

    return size;
}
        
std::size_t Index::getLenthAllDocs() {
    char temp[8];
    std::size_t size;
    std::filesystem::path path = indexPath_;
    path /= "docs" + std::to_string(numberDoc) + ".bin";

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open the file for reading");
    }

    file.seekg(-8, std::ios::end);
    file.read(temp, 8);
    std::memcpy(&size, temp, 8);

    return size;
}

std::size_t Index::FrequencyTerm(const std::size_t idx, const std::size_t DocId) {
    std::ifstream file(pathIndexG, std::ios::binary);
    std::size_t res = 0;

    char temp[8];
    std::size_t size;

    std::size_t cntDocs;
    file.read(temp, 8);
    std::memcpy(&cntDocs, temp, 8);
    file.seekg(idx * 8, std::ios::cur);
    std::size_t pos;
    file.read(temp, 8);
    std::memcpy(&pos, temp, 8);
    file.seekg(pos + 8 + cntDocs * 8, std::ios::beg);
    file.read(temp, 8);
    std::memcpy(&size, temp, 8);

    for (int i = 0; i < size; i++) {
        std::size_t tempDocId;
        file.read(temp, 8);
        std::memcpy(&tempDocId, temp, 8);

        std::size_t sizeList;
        file.read(temp, 8);
        std::memcpy(&sizeList, temp, 8);
        file.seekg(sizeList * 8, std::ios::cur);
        if (DocId == tempDocId) {
            res = sizeList;
            return res;
        }
    }

    return res;
}
