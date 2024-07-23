#include "parser.h"

bool ArgumentParser::Parse(int argc, char** argv) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    
    for (int i = 0; i < args.size(); i++) {
        if (args[i] == "-d" || args[i] == "--directory") {
            path = args[++i];
        } else if (args[i] == "-i" || args[i] == "--idxpath") {
            idx_path = args[++i];
        } else {
            return false;
        }
    }

    return true;
}

std::string ArgumentParser::GetDirPath() {
    return path;
}

std::string ArgumentParser::GetIdxPath() {
    return idx_path;
}