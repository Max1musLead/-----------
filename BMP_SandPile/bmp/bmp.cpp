#include "bmp.h"

int PaddingAmount(int width, int height) {
    int padding_amount = (4 - ((width / 2 + width % 2) % 4)) % 4;
    return padding_amount;
}

Image LoadFromDisk(char* load) {
    Image image;
    СolorPalette palete;
    BMPPixelField pixel_matrich;

    std::ifstream file(load, std::ifstream::binary);
    file.read(reinterpret_cast<char*>(image.bmfh.bytes), kBMPHeaderSize);
    file.read(reinterpret_cast<char*>(image.bmih.bytes), kBMPInfoSize);
    file.read(reinterpret_cast<char*>(palete.arrColor), kCntColors * kNumBytes);

    image.width = image.bmih.bytes[4] + (image.bmih.bytes[5] << 8) + (image.bmih.bytes[6] << 16) + (image.bmih.bytes[7] << 24);
    image.height = image.bmih.bytes[8] + (image.bmih.bytes[9] << 8) + (image.bmih.bytes[10] << 16) + (image.bmih.bytes[11] << 24);

    int padding_amount = PaddingAmount(image.width, image.height);
    int new_width = (image.width / 2 + image.height % 2) + padding_amount;

    pixel_matrich.array = new uint8_t*[image.height];
    for (int i = 0; i < image.height; i++) {
        pixel_matrich.array[i] = new uint8_t[new_width];
        std::memset(pixel_matrich.array[i], 0, new_width);
    }

    for (int i = 0; i < image.height; ++i) {
        file.read(reinterpret_cast<char*>(pixel_matrich.array[i]), new_width);
    }

    pixel_matrich.height = image.height;
    pixel_matrich.width = new_width;

    file.close();

    image.pixel_arr = pixel_matrich;

    return image;
}

Image DoBMPPixelField(uint64_t** matrich, int width, int height, Image img) {
    BMPPixelField pixel_array;
    int padding_amount = PaddingAmount(img.width, img.height);

    int new_width = (width / 2 + width % 2) + padding_amount;

    pixel_array.array = new uint8_t*[height];
    for (int i = 0; i < height; i++) {
        pixel_array.array[i] = new uint8_t[new_width];
        std::memset(pixel_array.array[i], 0, new_width);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j % 2 == 0) {
                pixel_array.array[height - i - 1][j / 2] += (matrich[i][j] > 3 ? 4 : matrich[i][j]) << 4;
            } else {
                pixel_array.array[height - i - 1][j / 2] += (matrich[i][j] > 3 ? 4 : matrich[i][j]);
            }
        }
    }
    pixel_array.width = new_width;
    pixel_array.height = height;

    img.pixel_arr = pixel_array;

    return img;
}

Image CreateBmp(int width, int height, uint64_t** matrich) {
    Image img;
    int padding_amount = PaddingAmount(width, height);
    const int file_size = kBMPHeaderSize + kBMPInfoSize + (width / 2 + width % 2) * height + padding_amount * height;
    BitMapFileHeader bfh;
	BitMapInfoHeader bih;

    // Тип файла
    bfh.bytes[0] = 'B';
    bfh.bytes[1] = 'M';
    // Размер файла
    bfh.bytes[2] = file_size;
    bfh.bytes[3] = file_size >> 8;
    bfh.bytes[4] = file_size >> 16;
    bfh.bytes[4] = file_size >> 24;
    // Reserved 1 - 0
    bfh.bytes[6] = 0;
    bfh.bytes[7] = 0;
    // Reserved 2 - 0
    bfh.bytes[8] = 0;
    bfh.bytes[9] = 0;
    // Pixel data offset
    bfh.bytes[10] = kBMPHeaderSize + kBMPInfoSize + kNumBytes * kCntColors;
    bfh.bytes[11] = 0;
    bfh.bytes[12] = 0;
    bfh.bytes[13] = 0;

    // Header size 0-3
    bih.bytes[0] = kBMPInfoSize;
    bih.bytes[1] = 0;
    bih.bytes[2] = 0;
    bih.bytes[3] = 0;
    // Ширина 4-7
    bih.bytes[4] = width;
    bih.bytes[5] = width >> 8;
    bih.bytes[6] = width >> 16;
    bih.bytes[7] = width >> 24;
    // Высота 8-11
    bih.bytes[8] = height;
    bih.bytes[9] = height >> 8;
    bih.bytes[10] = height >> 16;
    bih.bytes[11] = height >> 24;
    // Planes 12-13
    bih.bytes[12] = 1;
    // Битов на пиксель (RGB) 14-15
    bih.bytes[14] = 4;
    // Способ хранения пикселей  16-19
    // Размер данных о пикселях  20-23
    // Кол-во пиксельей по x  24-27
    // Кол-во пикселей по y  28-31
    // Количество цветов в палитре 32-35
    bih.bytes[32] = kCntColors;
    // Важные цвета - 0 36-39

    img.bmfh = bfh;
    img.bmih = bih;
    img.height = height;
    img.width = width;
    img.matrich = matrich;

    return img;
}

void SaveBMP(char* output, char* name, Image image) {
    СolorPalette palete;

    std::filesystem::path path = output;
    path /= name;
    path += ".bmp";
    
    std::ofstream file_output(path, std::ios::binary);
    file_output.write(reinterpret_cast<char*>(image.bmfh.bytes), kBMPHeaderSize);
    file_output.write(reinterpret_cast<char*>(image.bmih.bytes), kBMPInfoSize);
    file_output.write(reinterpret_cast<char*>(palete.arrColor), kCntColors * kNumBytes);

    for (int i = 0; i < image.pixel_arr.height; ++i) {
        file_output.write(reinterpret_cast<char*>(image.pixel_arr.array[i]), image.pixel_arr.width);
    }
}
