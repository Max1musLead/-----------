#include <iostream>
#include <cinttypes>

class uint17_t {
    public:
        operator int() const;

        uint17_t& operator=(int value);

        uint17_t(uint16_t* array, int index);

    private:
        uint16_t* array_;
        int index_;
};

class BigArray {
    private:
        uint16_t* array_;
        int x_size_;
        int y_size_;
        int z_size_;

        BigArray(int x, int y, int z);

        class SecondProcessing {
            public:
                uint17_t operator[] (int index);

                SecondProcessing(uint16_t* array, int index);

            private:
                uint16_t* array_;
                int index_;
        };

        class FirstProcessing {
            public:
                SecondProcessing operator[] (int index);

                FirstProcessing(uint16_t* array, int index, int z_size);

            private:
                uint16_t* array_;
                int index_;
                int zsize_;
        };

    public:
        BigArray(const BigArray& other);

        BigArray() = default;

        ~BigArray();

        BigArray& operator=(const BigArray& other);

        BigArray operator+(BigArray& rhs);

        BigArray operator-(BigArray& rhs);

        BigArray operator*(BigArray& rhs);
        
        BigArray operator*(int value);

        FirstProcessing operator[] (int index);

        static BigArray make_array(int x, int y, int z) {
            return {x, y, z};
        }

        int GetX() const { return x_size_; }

        int GetY() const { return y_size_; }

        int GetZ() const { return z_size_; }
};

std::ostream& operator<<(std::ostream& output, const uint17_t& value);
std::istream& operator>>(std::istream& input, uint17_t&& value);
