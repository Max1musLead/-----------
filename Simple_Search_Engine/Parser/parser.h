#pragma once

#include <vector>
#include <string>

class ArgumentParser {
public:
    bool Parse(int argc, char** argv);
    std::string GetDirPath();
    std::string GetIdxPath();

private:
    std::string path;
    std::string idx_path;
};