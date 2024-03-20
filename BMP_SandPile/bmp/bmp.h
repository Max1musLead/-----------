#pragma once

#include <cstring>
#include <cinttypes>
#include <fstream>
#include <filesystem>
#include <iostream>

const int kNumBytes = 4;
const int kCntColors = 5;
const int kBMPHeaderSize = 14;
const int kBMPInfoSize = 40;

struct BitMapFileHeader{
    uint8_t bytes[kBMPHeaderSize]{};
};

struct BitMapInfoHeader{
    uint8_t bytes[kBMPInfoSize]{};
};

struct BMPPixelField {
    uint8_t** array;
    int width = 0;
    int height = 0;
};

struct Image {
    BitMapFileHeader bmfh;
    BitMapInfoHeader bmih;
    uint64_t** matrich;
    BMPPixelField pixel_arr;
    int width = 0;
    int height = 0;

    uint64_t* operator[](uint64_t x) {
        return matrich[x];
    }

    uint64_t& operator()(int ind1, int ind2) {
        int new_height = height;
        int new_width = width;
        int move_down = 0;
        int move_right = 0;

        if (ind1 + 1 > height) {
            new_height = abs(ind1) + 1;
        }
        if (ind2 + 1 > width) {
            new_width = abs(ind2) + 1;
        }
        if (ind1 < 0) {
            new_height = height + abs(ind1);
            move_down = abs(ind1);
        }
        if (ind2 < 0) {
            new_width = width + abs(ind2);
            move_right = abs(ind2);
        }

        if (ind1 + 1 > height || ind2 + 1 > width || move_down != 0 || move_right != 0) {
            uint64_t** copy_arr = new uint64_t*[new_height];
            for (int i = 0; i < new_height; ++i) {
                copy_arr[i] = new uint64_t[new_width];
                std::memset(copy_arr[i], 0, new_width * 8);
            }
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    copy_arr[i + move_down][j + move_right] = matrich[i][j];
                }
            }
            for (int i = 0; i < height; ++i) {
                delete[] matrich[i];
            }
            delete[] matrich;

            matrich = copy_arr;
        }

        height = new_height;
        width = new_width;

        return matrich[ind1 + move_down][ind2 + move_right];
    }
};

struct СolorPalette {
    uint8_t arrColor[kNumBytes * kCntColors]
    {255, 255, 255, 0, // White
     0, 255, 0, 0, // Green
     0, 255, 255, 0, // Yelow
     255, 0, 255, 0, // Purple
     0, 0, 0, 0 // Black
    };
};

Image DoBMPPixelField(uint64_t** matrich, int width, int height, Image img);

Image CreateBmp(int width, int height, uint64_t** matrich);

void SaveBMP(char* output, char* name, Image image);

Image LoadFromDisk(char* load);
