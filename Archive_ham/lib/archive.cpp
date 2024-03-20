#include "archive.h"

void archived_file::CreateArchive(const std::vector<std::string>& file_names, const std::string& name) {
    name_ = name;
    std::filesystem::path path = path_;
    path /= name_;
    path += ".haf";
    
    std::ofstream file_output(path, std::ios::binary);
    
    for (int i = 0; i < file_names.size(); i++) {
        std::ifstream temp(file_names[i], std::ios::binary);
        if (temp) {
            Header head;
            int file_size;
            std::vector<char> binary_file = ToBin(file_names[i]);

            file_size = binary_file.size() + CountControlBits(binary_file.size());
            head.filename = file_names[i];

            head.size[0] = file_size;
            head.size[1] = file_size >> 8;
            head.size[2] = file_size >> 16;
            head.size[3] = file_size >> 24;

            file_output.write(reinterpret_cast<char*>(head.size), kCntByteSize);

            file_output.put(head.filename.size());

            for (int i = 0; i < head.filename.size(); i++) {
                file_output.put(head.filename[i]);
            }

            std::vector<char> ham_code = GenerateHamming(binary_file);
            
            for (int i = 0; i < ham_code.size(); i++) {
                file_output.put(ham_code[i]);
            }
        }
        temp.close();
    }
    file_output.close();
}

void archived_file::DeleteFile(const std::string& archive, const std::string& filename) {
    std::filesystem::path p = archive.substr(0, archive.size() - 4);
    p += "1.haf";
    std::ifstream file(archive, std::ios::binary);
    std::ofstream new_file(p, std::ios::binary);
    
    while (file.peek() != EOF) {
        Header head;
        char ch;

        file.read(reinterpret_cast<char*>(head.size), kCntByteSize);
        int size_file = head.size[0] + (head.size[1] << 8) + (head.size[2] << 16) + (head.size[3] << 24);
        
        file.get(ch);
        int size_text = static_cast<int>(ch);

        for (int i = 0; i < size_text; i++) {
            file.get(ch);
            head.filename += ch;
        }

        if (head.filename == filename) {
            file.seekg(size_file, std::ios::cur);
        } else {
            new_file.write(reinterpret_cast<char*>(head.size), kCntByteSize);

            new_file.put(head.filename.size());

            for (int i = 0; i < head.filename.size(); i++) {
                new_file.put(head.filename[i]);
            }

            for (int i = 0; i < size_file; i++) {
                file.get(ch);
                new_file.put(ch);
            }
        }
    }

    file.close();
    new_file.close();

    std::ofstream file2(archive, std::ios::trunc|std::ios::binary);
    std::ifstream new_file2(p, std::ios::binary);

    char ch;

    while (!new_file2.eof()) {
        new_file2.get(ch);
        file2.put(ch);
    }

    file2.close();
    new_file2.close();

    remove(p);
}

void archived_file::ListOfFiles(const std::string name) {
    std::ifstream file(name, std::ios::binary);

    while (file.peek() != EOF) {
        Header head;
        char ch;

        file.read(reinterpret_cast<char*>(head.size), kCntByteSize);
        int size_file = head.size[0] + (head.size[1] << 8) + (head.size[2] << 16) + (head.size[3] << 24);
        
        file.get(ch);
        int size_text = static_cast<int>(ch);

        for (int i = 0; i < size_text; i++) {
            file.get(ch);
            head.filename += ch;
        }

        std::cout << head.filename << "\n";

        file.seekg(size_file, std::ios::cur);
    }

    file.close();
}

void archived_file::ExtractFile(const std::string& arch_name, const std::vector <std::string>& filenames) {
    std::ifstream file(arch_name, std::ios::binary);

    if (filenames.size() == 0) {
        while (file.peek() != EOF) {
            std::vector <char> binary_file;
            Header head;
            char ch;
            
            file.read(reinterpret_cast<char*>(head.size), kCntByteSize);
            int size_file = head.size[0] + (head.size[1] << 8) + (head.size[2] << 16) + (head.size[3] << 24);
            
            file.get(ch);
            int size_text = static_cast<int>(ch);

            for (int i = 0; i < size_text; i++) {
                file.get(ch);
                head.filename += ch;
            }
            for (int i = 0; i < size_file; i++) {
                file.get(ch);
                binary_file.push_back(ch);
            }

            std::vector <char> decoder = DecodeFile(binary_file);

            std::ofstream new_file(head.filename, std::ios::binary);

            for (int i = 0; i < decoder.size(); i++) {
                new_file.put(decoder[i]);
            }
        }
    } else {
        for (int i = 0; i < filenames.size(); i++) {
            while (file.peek() != EOF) {
                std::vector <char> binary_file;
                Header head;
                char ch;
                
                file.read(reinterpret_cast<char*>(head.size), kCntByteSize);
                int size_file = head.size[0] + (head.size[1] << 8) + (head.size[2] << 16) + (head.size[3] << 24);
                
                file.get(ch);
                int size_text = static_cast<int>(ch);

                for (int i = 0; i < size_text; i++) {
                    file.get(ch);
                    head.filename += ch;
                }

                if (filenames[i] == head.filename) {
                    for (int i = 0; i < size_file; i++) {
                        file.get(ch);
                        binary_file.push_back(ch);
                    }

                    std::vector <char> decoder = DecodeFile(binary_file);

                    std::ofstream new_file(head.filename, std::ios::binary);

                    for (int i = 0; i < decoder.size(); i++) {
                        new_file.put(decoder[i]);
                    }

                } else {
                    file.seekg(size_file, std::ios::cur);
                }
            }
        }
    }

    file.close();
}

void archived_file::AddFile(const std::string& name_archive, const std::string& name_file) {
    std::ifstream f_archive(name_archive, std::ios::binary);
    std::filesystem::path p(name_archive);

    char ch;
    std::vector<std::string> names;

    while (f_archive.peek() != EOF) {
        Header head;
        char ch;

        f_archive.read(reinterpret_cast<char*>(head.size), kCntByteSize);
        int size_file = head.size[0] + (head.size[1] << 8) + (head.size[2] << 16) + (head.size[3] << 24);
        
        f_archive.get(ch);
        int size_text = static_cast<int>(ch);

        for (int i = 0; i < size_text; i++) {
            f_archive.get(ch);
            head.filename += ch;
        }

        names.push_back(head.filename);

        f_archive.seekg(size_file, std::ios::cur);
    }

    names.push_back(name_file);

    CreateArchive(names, p.stem().string());

    f_archive.close();
}

void archived_file::MergeArchives(const std::string& narchive, const std::string& first_archive, const std::string& second_archive) {
    std::filesystem::path path = path_;
    path /= narchive;
    path += ".haf";

    std::ofstream merge_archive(path, std::ios::binary);
    std::ifstream file_archive1(first_archive, std::ios::binary);
    std::ifstream file_archive2(second_archive, std::ios::binary);
    char ch;

    while (!file_archive1.eof()) {
        file_archive1.get(ch);
        merge_archive.put(ch);
    }

    while (!file_archive2.eof()) {
        file_archive2.get(ch);
        merge_archive.put(ch);
    }

    merge_archive.close();
    file_archive1.close();
    file_archive2.close();
}

int CountControlBits(int lenth) {
    int cnt = 1;

    while (pow(2, cnt) < (lenth + cnt + 1)) {
        cnt++;
    }
    
    return cnt;
}

std::vector<char> ToBin(const std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> binary_file;
    char ch;

    while (file.get(ch)) {
        std::bitset<8> bin(ch);
        for (int i = 7; i >= 0; --i) {
            if (bin[i]) {
                binary_file.push_back('1');
            } else {
                binary_file.push_back('0');
            }
        }
    }

    file.close();

    return binary_file;
}

bool FixErrors(std::vector<char>& ham_code) {
    int control_bits = CountControlBits(ham_code.size() - CountControlBits(ham_code.size()));
    int num_error = 0;

    for (int i = 0; i < control_bits; ++i) {
        int qur_bit = pow(2, i);
        int dop_bit = ham_code[qur_bit - 1] - '0';
        int bit = 0;
        for (int j = qur_bit - 1; j < ham_code.size(); j += 2 * qur_bit) {
            for (int g = j; (g < j + qur_bit) && (g < ham_code.size()); ++g) {
                bit ^= (ham_code[g] - '0');
            }
        }
        if ((bit ^ dop_bit) != ham_code[qur_bit - 1] - '0') {
            num_error += qur_bit;
        }
    }

    if (num_error == 0) {
        return true;
    } else if (num_error <= ham_code.size()) {
        if (ham_code[num_error - 1] == '0') {
            ham_code[num_error - 1] = '1';
        } else {
            ham_code[num_error - 1] = '0';
        }
        return true;
    }

    return false;
}

std::vector<char> DecodeFile(std::vector<char>& binary_code) {
    std::vector <char> decode_code;
    std::vector <char> decode_file;
    int cnt = 0;
    if (FixErrors(binary_code)) {
        for (int i = 0; i < binary_code.size(); ++i) {
            if ((pow(2, cnt) - 1) == i) {
                cnt++;
            } else {
                decode_code.push_back(binary_code[i]);
            }
        }
    } else {
        std::cerr << "It is impossible to restore: Erros > 1";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < decode_code.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8 && (i + j) < decode_code.size(); ++j) {
            byte <<= 1;
            byte += decode_code[i + j] - '0';
        }
        decode_file.push_back(byte);
    }

    return decode_file;
}

std::vector<char> GenerateHamming(const std::vector<char>& binary_code) {
    int control_bits = CountControlBits(binary_code.size());
    std::vector<char> ham_code(binary_code.size() + control_bits, '0');

    int j = 0;
    int g = 0;

    for (int i = 0; i < ham_code.size(); ++i) {
        if ((i + 1) != (pow(2, g))) {
            ham_code[i] = binary_code[j++];
        } else {
            g++;
        }
    }

    for (int i = 0; i < control_bits; ++i) {
        int qur_bit = pow(2, i);
        int bit = 0;
        for (int j = qur_bit - 1; j < ham_code.size(); j += 2 * qur_bit) {
            for (int g = j; (g < j + qur_bit) && (g < ham_code.size()); ++g) {
                bit ^= (ham_code[g] - '0');
            }
        }
        ham_code[qur_bit - 1] = (bit + '0');
    }

    return ham_code; 
}
