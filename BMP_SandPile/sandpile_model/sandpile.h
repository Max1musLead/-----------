#pragma once

#include <cinttypes>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

struct Sandpile {
    uint64_t** matrich;
    int16_t width;
    int16_t height;
};

void InitSandpileFromTsv(char* filename, Sandpile& sandpile);

void SandpileAllСrumble(Sandpile& sandpile);

bool SandpileСrumble(Sandpile& sandpile);

void SandpileExpand(Sandpile& sandpile);

void PrintSandpile(Sandpile& sandpile);