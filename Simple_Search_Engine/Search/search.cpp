#include "search.h"

std::list<std::pair<std::size_t, std::size_t>> readInfo(std::size_t idx) {
    std::ifstream file(pathIndexG, std::ios::binary);
    std::list<std::pair<std::size_t, std::size_t>> res;

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
        std::pair<std::size_t, std::size_t> tempDocList;
        file.read(temp, 8);
        std::memcpy(&tempDocList.first, temp, 8);
        tempDocList.second = file.tellg();
        res.push_back(std::make_pair(tempDocList.first, tempDocList.second));

        std::size_t sizeList;
        file.read(temp, 8);
        std::memcpy(&sizeList, temp, 8);
        file.seekg(sizeList * 8, std::ios::cur);
    }

    return res;
}

std::list<std::size_t> getListPos(const std::size_t idx) {
    char temp[8];
    std::list<std::size_t> res;
    std::ifstream file(pathIndexG, std::ios::binary);

    file.seekg(idx, std::ios::beg);

    std::size_t sizeList;
    file.read(temp, 8);
    std::memcpy(&sizeList, temp, 8);

    for (std::size_t i = 0; i < sizeList; i++) {
        std::size_t pos;
        file.read(temp, 8);
        std::memcpy(&pos, temp, 8);
        res.push_back(pos);
    }
    
    return res;
}

///// ОСНОВНАЯ ФУНКЦИЯ //////
std::vector<andAll> Search::search(const std::vector<std::string>& words, int checkAlls) {
    prqueue res;
    checkAlls = 0;
    if (checkAlls == 1) {
        res = searchAND(words);
    } else if (checkAlls == 2) {
        res = searchOR(words);
    } else {
        res = searchAl(words);
    }
    
    std::vector<andAll> elements;

    while (!res.empty()) {
        elements.push_back(res.top());
        res.pop();
    }

    std::reverse(elements.begin(), elements.end());
    
    for (const auto& element : elements) {
        double metrica = std::get<0>(element);
        std::size_t docID = std::get<1>(element);
        const std::list<std::size_t>& positions = std::get<2>(element);

        std::cout << "Metrica: " << metrica << ", Doc: " << index_.getDocumentName(docID) << ", Positions: ";
        for (const auto& pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << "\n";
    }

    return elements;
}

prqueue Search::searchAl(const std::vector<std::string>& terms) {
    std::stack<std::priority_queue<andAll, std::vector<andAll>, Comparator>> stack;
    std::vector<std::string> combinedTerms;
    
    for (const std::string& term : terms) {
        if (term == "AND" || term == "OR") {
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid expression");
            }
            auto right = stack.top();
            stack.pop();
            auto left = stack.top();
            stack.pop();

            std::priority_queue<andAll, std::vector<andAll>, Comparator> combinedResults;

            if (term == "AND") {
                combinedResults = searchAND(left, right);
            } else if (term == "OR") {
                combinedResults = searchOR(left, right);
            }

            stack.push(combinedResults);
        } else {
            std::vector<std::string> oneTerm = {term};
            stack.push(searchAND(oneTerm));
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return stack.top();
}

////// Merge AND //////
prqueue Search::searchAND(prqueue& left, prqueue& right) {
    prqueue result;
    std::unordered_map<std::size_t, andAll> mapLeft;
    std::unordered_map<std::size_t, andAll> mapRight;

    while (!left.empty()) {
        mapLeft[std::get<1>(left.top())] = left.top();
        left.pop();
    }

    while (!right.empty()) {
        mapRight[std::get<1>(right.top())] = right.top();
        right.pop();
    }

    for (const auto& item : mapLeft) {
        auto it = mapRight.find(item.first);
        if (it != mapRight.end()) {
            double combinedMetric = std::get<0>(item.second) + std::get<0>(it->second);
            std::list<std::size_t> combinedList = std::get<2>(item.second);
            combinedList.insert(combinedList.end(), std::get<2>(it->second).begin(), std::get<2>(it->second).end());
            result.push(std::make_tuple(combinedMetric, item.first, combinedList));
        }
    }

    return result;
}

////// Merge OR //////
prqueue Search::searchOR(prqueue& left, prqueue& right) {
    prqueue result;
    std::unordered_map<std::size_t, andAll> combinedMap;

    while (!left.empty()) {
        combinedMap[std::get<1>(left.top())] = left.top();
        left.pop();
    }

    while (!right.empty()) {
        std::size_t docID = std::get<1>(right.top());
        if (combinedMap.find(docID) != combinedMap.end()) {
            double combinedMetric = std::get<0>(combinedMap[docID]) + std::get<0>(right.top());
            std::list<std::size_t> combinedList = std::get<2>(combinedMap[docID]);
            combinedList.insert(combinedList.end(), std::get<2>(right.top()).begin(), std::get<2>(right.top()).end());
            combinedMap[docID] = std::make_tuple(combinedMetric, docID, combinedList);
        } else {
            combinedMap[docID] = right.top();
        }
        right.pop();
    }

    for (const auto& item : combinedMap) {
        result.push(item.second);
    }

    return result;
}


///// ALL AND //////
prqueue Search::searchAND(const std::vector<std::string>& terms) {
    prqueue res;
    std::map<std::size_t, std::list<std::pair<std::size_t, std::size_t>>> allDocs; // termID, list <DocID, ListSize>
    std::vector<std::pair<std::size_t, std::_List_iterator<std::pair<std::size_t, std::size_t>>>> iterators; // termID, Iterlist <DocID, ListSize>

    for (std::size_t i = 0; i < terms.size(); i++) {
        if (auto j = index_.getTermId(terms[i]); j.second && !(terms[i] == "AND" || terms[i] == "OR" || terms[i] == "(" || terms[i] == ")")) {
            std::size_t termId = j.first;
            allDocs[termId] = readInfo(termId);
            iterators.push_back(std::make_pair(termId, allDocs[termId].begin()));
        } else if (terms[i] == "AND" || terms[i] == "OR" || terms[i] == "(" || terms[i] == ")") {
            continue;
        } else {
            std::cerr << terms[i];
            throw std::runtime_error("Incorrect term");
        }
    }

    if (iterators.empty()) return res;

    while (true) {
        std::sort(iterators.begin(), iterators.end(), [&](const auto& a, const auto& b) {
            return a.second->first < b.second->first;
        });

        std::size_t currentDID = iterators[0].second->first;
        bool allMatch = true;

        for (auto& it : iterators) {
            while (it.second != allDocs[it.first].end() && it.second->first < currentDID) {
                ++it.second;
            }
            if (it.second == allDocs[it.first].end() || it.second->first != currentDID) {
                allMatch = false;
                break;
            }
        }

        if (allMatch) {
            std::list<std::size_t> positions;
            double score = 0.0;
            for (auto& it : iterators) {
                score += BM25TFIDF(it.first, it.second->first);
                std::list<std::size_t> tempPositions = getListPos(it.second->second);
                for (const auto& pos : tempPositions) {
                    positions.push_back(pos);
                }
            }
            res.emplace(score, currentDID, positions);

            if (res.size() > MAX_RESULTS) { res.pop(); }
        }

        bool end = true;
        for (auto& it : iterators) {
            if (++it.second != allDocs[it.first].end()) {
                end = false;
                break;
            }
        }

        if (end) { 
            break;
        }
    }

    return res;
}

///// ALL OR //////
prqueue Search::searchOR(const std::vector<std::string>& terms) {
    prqueue res;
    std::map<std::size_t, std::list<std::pair<std::size_t, std::size_t>>> allDocs; // termID, list <DocID, ListSize>
    std::vector<std::pair<std::size_t, std::_List_iterator<std::pair<std::size_t, std::size_t>>>> iterators; // termID, Iterlist <DocID, ListSize>

    for (std::size_t i = 0; i < terms.size(); i++) {
        if (auto j = index_.getTermId(terms[i]); j.second && !(terms[i] == "AND" || terms[i] == "OR" || terms[i] == "(" || terms[i] == ")")) {
            std::size_t termId = j.first;
            allDocs[termId] = (readInfo(termId));
            iterators.push_back(std::make_pair(termId, allDocs[termId].begin()));
        } else if (terms[i] == "AND" || terms[i] == "OR" || terms[i] == "(" || terms[i] == ")") {
            continue;
        } else {
            std::cerr << terms[i];
            throw std::runtime_error("Incorrect term");
        }
    }

    while (!iterators.empty()) {
        std::sort(iterators.begin(), iterators.end(), [&](const auto& a, const auto& b) 
        { return a.second->first < b.second->first; });
        std::size_t currentDID = iterators[0].second->first;

        std::list<std::size_t> lines;
        double score = 0.0;
        
        std::list<size_t> toIncrementIdx;
        for (size_t idx = 0; idx < iterators.size(); ++idx) {
            std::list<std::size_t> tempLines;
            if (iterators[idx].second->first == currentDID) {
                score += BM25TFIDF(iterators[idx].first, iterators[idx].second->first);
                tempLines = getListPos(iterators[idx].second->second);
                for (const auto& pos : tempLines) {
                    lines.push_back(pos);
                }
                toIncrementIdx.push_back(idx);
            }
        }

        res.emplace(score, currentDID, lines);
        if (res.size() > MAX_RESULTS) { res.pop(); } 

        for (auto it = toIncrementIdx.rbegin(); it != toIncrementIdx.rend(); ++it) {
            auto& iter = iterators[*it];
            if (++(iter.second) == allDocs[iter.first].end()) {
                iterators.erase(iterators.begin() + *it);
            }
        }
    }
    
    return res;
}

double Search::TFIDF(const std::size_t termId, const std::size_t docID) { /// tf = количество вхождений терма в документ
    std::size_t allDocs = index_.getDocsCount();
    std::size_t termDocSize = index_.getDocsCountToTerm(termId);
    std::size_t tf = index_.FrequencyTerm(termId, docID);

    return (tf * std::log((allDocs / termDocSize)));
}

double Search::BM25TFIDF(const std::size_t termId, const std::size_t docID) {
    const std::size_t docLength = index_.getDocumentSize(docID);
    std::size_t avgDl = index_.getLenthAllDocs() / docLength;

    // TF
    double termFrequency = index_.FrequencyTerm(termId, docID);
    
    // IDF from TF-IDF
    double N = index_.getDocsCount();
    double n = index_.getDocsCountToTerm(termId);
    double idf = std::log((N / (n + 1)) + 1);
    
    // BM25 parameters
    double k1 = 1.5;
    double b = 0.75;
    
    // BM25 calculation using TF-IDF - IDF
    double tf = termFrequency / (termFrequency + k1 * (1 - b + b * (docLength / avgDl)));
    return idf * (tf * (k1 + 1));
}
