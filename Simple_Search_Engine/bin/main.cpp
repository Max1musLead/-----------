#include "Index/index.h"
#include "Parser/parser.h"
#include <chrono>

int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();
    
    ArgumentParser Parser;

    if (!Parser.Parse(argc, argv)){
        exit(EXIT_FAILURE);
    }

    Index index(Parser.GetIdxPath());

    Process processer(Parser.GetDirPath(), index);

    processer.buildIndex();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}
