#include "parser.h"

void CheckIntError(char* endptr, char* argument) {
    if (endptr == argument || *endptr != '\0' || errno != 0) {
        std::cerr << "Please parse correct argument";
        exit(EXIT_FAILURE);
    }
}

void CheckLimits(int i, int argc) {
    if (i + 1 == argc){
        std::cerr << "No link to the file";
        exit(EXIT_FAILURE);
    }
}

Input ParseArgumetns(int argc, char** argv) {
    Input input;
    char* endptr;
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (strcmp(arg, "-i") == 0 || strcmp(arg, "--input") == 0) { 
            CheckLimits(i, argc);
            input.filename = (argv[i + 1]);
            if (!std::filesystem::exists(input.filename)) {
                std::cerr << "File opening error";
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--output") == 0) {
            CheckLimits(i, argc);
            input.output = (argv[i + 1]);
        } else if (strcmp(arg, "-l") == 0 || strcmp(arg, "--load") == 0) {
            CheckLimits(i, argc);
            input.load = (argv[i + 1]);
            if (!std::filesystem::exists(input.load)) {
                std::cerr << "File opening error";
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--max-iter") == 0) {
            CheckLimits(i, argc);
            input.iter = strtol(argv[i + 1], &endptr, 10);
            CheckIntError(endptr, argv[i + 1]);
        } else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--freq") == 0 ) {
            CheckLimits(i, argc);
            input.freq = strtol(argv[i + 1], &endptr, 10);
            CheckIntError(endptr, argv[i + 1]);
        }
    }

    return input;
}
