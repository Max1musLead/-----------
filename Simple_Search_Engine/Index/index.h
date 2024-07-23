#pragma once

#include <algorithm>
#include <cstring>
#include <cctype>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <list>
#include <fstream>
#include <memory>

const int maxCountRecords = 1000000;
const std::string pathIndexG = "C:/Users/Maksi/github-classroom/is-itmo-c-23/labwork11-Max1musLead/invertIndex.bin";
using documentID = std::pair<std::size_t, std::filesystem::path>;

struct Data {
    std::size_t first;
    std::size_t second;
    std::shared_ptr<std::list<std::size_t>> third;
};

///// INDEX /////
class Index {
public:
    Index(std::filesystem::path idxPath) : indexPath_(idxPath) {};

    void uploadDataToDisk(const std::vector<Data>& dataArray);

    void uploadDocumentsToDisk(const std::vector<documentID>& documents);
    
    void uploadTermsToDisk(const std::unordered_map<std::string, std::size_t>& terms);

    void mergeBlocks();

    std::string getDocumentName(std::size_t idx);

    std::size_t getDocumentSize(std::size_t idx);

    std::pair<std::size_t, bool> getTermId(const std::string& term);

    std::size_t getPostLIstSz(std::size_t term);

    std::size_t getDocsCount();

    std::size_t getLenthAllDocs();

    std::size_t getDocsCountToTerm(std::size_t idx);

    std::size_t FrequencyTerm(const std::size_t idx, const std::size_t DocId);

private:
    std::size_t number_block = 100;
    std::size_t numberDoc = 1;
    std::filesystem::path indexPath_;
    std::size_t allWords = 0;
};

///// PROCESSER /////
class Process {
public:
    Process(const std::string& path, Index& index) : directoryPath_(path), dirIterator_(std::filesystem::recursive_directory_iterator(path)), index_(index) {}

    void buildIndex();
    bool isFinished();

private:
    void processFile(const std::string& filePath);
    void clearTables();

    std::filesystem::path directoryPath_;
    std::filesystem::recursive_directory_iterator dirIterator_;
    std::size_t docID = 0;
    Index& index_;
    std::vector<Data> dataArray_;
    std::vector<documentID> documents_;
    std::unordered_map<std::string, std::size_t> terms_;
};
