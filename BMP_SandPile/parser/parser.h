#pragma once

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <filesystem>

struct Input{
    char* filename;
    char* output;
    char* load;
    int iter = 0;
    int freq = 0;
    int cnt_iter = 0;
    int cnt_freq = 0;
    bool is_stable = false;
};

Input ParseArgumetns(int argc, char** argv);
