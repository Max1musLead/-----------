#pragma once

#include <bitset>
#include <cmath>
#include <cstring>
#include <cinttypes>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

const uint16_t kCntByteSize = 4;

struct Header {
    uint8_t size[4];
    std::string filename;
};

class archived_file {
    public:
        void CreateArchive(const std::vector<std::string>& file_names, const std::string& name);
        void DeleteFile(const std::string& archive, const std::string& file);
        void ListOfFiles(const std::string name);
        void ExtractFile(const std::string& arch_name, const std::vector <std::string>& filenames);
        void AddFile(const std::string& name_archive, const std::string& name_file);
        void MergeArchives(const std::string& narchive, const std::string& first_archive, const std::string& second_archive);
        
    private:  
        std::string name_ = "good";
        std::string path_ = "C:/Users/Maksim/github-classroom/is-itmo-c-23/labwork6-Max1musLead/";
};

std::vector<char> ToBin(const std::string filename);

std::vector<char> DecodeFile(std::vector<char>& binary_file);

std::vector<char> GenerateHamming(const std::vector<char>& binary_code);

bool FixErrors(std::vector<char>& binary_code);

int CountControlBits(int lenth);
