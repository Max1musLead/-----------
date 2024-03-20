#include "3DArray.h"

uint17_t::uint17_t(uint16_t* array, int index) {
    array_ = array;
    index_ = index;
}

uint17_t::operator int() const {
    int index = index_ / 16;
    int res1 = array_[index] >> (index_ % 16);
    int res2 = (array_[index + 1] & ((1 << 16) - 1) >> (15 - (index_ % 16)));
    res2 <<= 16 - (index_ % 16);

    return res1 + res2;
}

uint17_t& uint17_t::operator=(int value) {
    int index = index_ / 16;

    array_[index] = (array_[index] & ((1 << 16) - 1) >> (16 - (index_ % 16)));
    array_[index] += (value << (index_ % 16));

    value >>= 16 - (index_ % 16);
    
    array_[index + 1] = (array_[index + 1] >> ((index_ % 16) + 1)) << ((index_ % 16) + 1);
    array_[index + 1] += (value & ((1 << 16) - 1) >> (16 - (index_ % 16 + 1)));

    return *this;
}

BigArray::BigArray(int x, int y, int z) {
    x_size_ = x;
    y_size_ = y;
    z_size_ = z;
    int size = x_size_ * y_size_ * z_size_ * 17 / 16;

    if (x_size_ * y_size_ * z_size_ * 17 % 16 != 0) {
        size++;
    }

    array_ = new uint16_t[size];
}

BigArray::~BigArray() {
    delete[] array_;
    array_ = nullptr;
}

BigArray::BigArray(const BigArray& other){
    x_size_ = other.x_size_;
    y_size_ = other.y_size_;
    z_size_ = other.z_size_;
    int size = x_size_ * y_size_ * z_size_ * 17 / 16;

    if (x_size_ * y_size_ * z_size_ * 17 % 16 != 0) {
        size++;
    }

    array_ = new uint16_t[size];

    for (int i = 0; i < size; i++) {
        array_[i] = other.array_[i];
    }
}

BigArray BigArray::operator*(BigArray& rhs) {
    BigArray NewArr = BigArray::make_array(rhs.GetX(), rhs.GetY(), rhs.GetZ());

    for (int i = 0; i < rhs.GetX(); i++) {
        for (int j = 0; j < rhs.GetY(); j++) {
            for (int k = 0; k < rhs.GetZ(); k++) {
                NewArr[i][j][k] = static_cast<int>((*this)[i][j][k]) * static_cast<int>(rhs[i][j][k]);
            }
        }
    }
    
    return NewArr;
}

BigArray BigArray::operator+(BigArray& rhs) {
    BigArray NewArr = BigArray::make_array(rhs.GetX(), rhs.GetY(), rhs.GetZ());

    for (int i = 0; i < rhs.GetX(); i++) {
        for (int j = 0; j < rhs.GetY(); j++) {
            for (int k = 0; k < rhs.GetZ(); k++) {
                NewArr[i][j][k] = static_cast<int>((*this)[i][j][k]) + static_cast<int>(rhs[i][j][k]);
            }
        }
    }
    
    return NewArr;
}

BigArray BigArray::operator-(BigArray& rhs) {
    BigArray NewArr = BigArray::make_array(rhs.GetX(), rhs.GetY(), rhs.GetZ());

    for (int i = 0; i < rhs.GetX(); i++) {
        for (int j = 0; j < rhs.GetY(); j++) {
            for (int k = 0; k < rhs.GetZ(); k++) {
                NewArr[i][j][k] = static_cast<int>((*this)[i][j][k]) - static_cast<int>(rhs[i][j][k]);
            }
        }
    }
    
    return NewArr;
}

BigArray BigArray::operator*(int value) {
    BigArray NewArr = BigArray::make_array((*this).GetX(), (*this).GetY(), (*this).GetZ());

    for (int i = 0; i < (*this).GetX(); i++) {
        for (int j = 0; j < (*this).GetY(); j++) {
            for (int k = 0; k < (*this).GetZ(); k++) {
                NewArr[i][j][k] = static_cast<int>((*this)[i][j][k]) * value;
            }
        }
    }

    return NewArr;
}

BigArray& BigArray::operator=(const BigArray& other) {
    x_size_ = other.x_size_;
    y_size_ = other.y_size_;
    z_size_ = other.z_size_;

    int size = x_size_ * y_size_ * z_size_ * 17 / 16;

    if (x_size_ * y_size_ * z_size_ * 17 % 16 != 0) {
        size++;
    }

    array_ = new uint16_t[size];

    for (int i = 0; i < size; i++) {
        array_[i] = other.array_[i];
    }
    
    return *this;
}

BigArray::FirstProcessing BigArray::operator[] (int index) {
    return {array_, (index * x_size_ * y_size_), z_size_};
}

BigArray::FirstProcessing::FirstProcessing(uint16_t* array, int index, int z_size) {
    array_ = array;
    zsize_ = z_size;
    index_ = index;
}

BigArray::SecondProcessing BigArray::FirstProcessing::operator[] (int index) {
    return {array_, index_ + (zsize_ * index)};
}

BigArray::SecondProcessing::SecondProcessing(uint16_t* array, int index) {
    array_ = array;
    index_ = index;
}

uint17_t BigArray::SecondProcessing::operator[] (int index) {
    return {array_, (index_ + index) * 17};
}

std::ostream& operator<<(std::ostream& output, const uint17_t& value) {
    output << static_cast<int>(value);

    return output;
}

std::istream& operator>>(std::istream& input, uint17_t&& value) {
    int in;
    input >> in;
    value = in;

    return input;
}
