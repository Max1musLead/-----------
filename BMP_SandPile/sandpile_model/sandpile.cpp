#include "sandpile.h"

struct Tsv {
    int16_t x;
    int16_t y;
    int16_t min_x = 30000;
    int16_t max_x = -30000;
    int16_t min_y = 30000;
    int16_t max_y = -30000;
    uint64_t cnt_pes;
};

void InitSandpileFromTsv(char* filename, Sandpile& sandpile) {
    Tsv tsv;
    std::ifstream file(filename);

    while (file >> tsv.x >> tsv.y >> tsv.cnt_pes) {
        tsv.min_x = std::min(tsv.min_x, tsv.x);
        tsv.min_y = std::min(tsv.min_y, tsv.y);
        tsv.max_x = std::max(tsv.max_x, tsv.x);
        tsv.max_y = std::max(tsv.max_y, tsv.y);
    }

    sandpile.width = tsv.max_x - tsv.min_x + 1;
    sandpile.height = tsv.max_y - tsv.min_y + 1;
    
    uint64_t** matrich = new uint64_t*[sandpile.height];

    for (int i = 0; i < sandpile.height; ++i) {
        matrich[i] = new uint64_t[sandpile.width];
        std::memset(matrich[i], 0, sandpile.width * 8);
    }

    sandpile.matrich = matrich;

    file.close();
    file.open(filename);

    while (file >> tsv.x >> tsv.y >> tsv.cnt_pes){
        sandpile.matrich[sandpile.height - tsv.y + tsv.min_y - 1][tsv.x - tsv.min_x] = tsv.cnt_pes;
    }

    file.close();
}

void SandpileAllСrumble(Sandpile& sandpile) {
    bool is_stable = true;

    while (is_stable) {
        is_stable = false;
        SandpileExpand(sandpile);
        for (int i = 1; i < sandpile.height - 1; ++i) {
            for (int j = 1; j < sandpile.width - 1; ++j) {
                if (sandpile.matrich[i][j] > 3) {
                    is_stable = true;
                    sandpile.matrich[i + 1][j] += sandpile.matrich[i][j] / 4;
                    sandpile.matrich[i - 1][j] += sandpile.matrich[i][j] / 4;
                    sandpile.matrich[i][j + 1] += sandpile.matrich[i][j] / 4;
                    sandpile.matrich[i][j - 1] += sandpile.matrich[i][j] / 4;
                    sandpile.matrich[i][j] %= 4;
                }
            }
        }
    }
}

bool SandpileСrumble(Sandpile& sandpile) {
    bool is_stable = true;

    SandpileExpand(sandpile);
    for (int i = 1; i < sandpile.height - 1; ++i) {
        for (int j = 1; j < sandpile.width - 1; ++j) {
            if (sandpile.matrich[i][j] > 3) {
                is_stable = false;
                sandpile.matrich[i][j] -= 4;
                sandpile.matrich[i + 1][j] += 1;
                sandpile.matrich[i - 1][j] += 1;
                sandpile.matrich[i][j + 1] += 1;
                sandpile.matrich[i][j - 1] += 1;
            }
        }
    }

    return is_stable;
}

void PrintSandpile(Sandpile& sandpile) {
    for (int i = 0; i < sandpile.height; i++) {
        for (int j = 0; j < sandpile.width; j++) {
            std::cout << sandpile.matrich[i][j];
        }
        std::cout << std::endl;
    }
}

void SandpileExpand(Sandpile& sandpile) {
    int right = 0;
    int left = 0;
    int up = 0;
    int down = 0;
    int diff_height = 0;
    int diff_width = 0;

    for (int i = 0; i < sandpile.width; i++){
        if (sandpile.matrich[sandpile.height - 1][i] >= 4) {
            up = 1;
        } 
        if (sandpile.matrich[0][i] >=  4) {
            down = 1;
            diff_height = 1;
        }
    }

    for (int i = 0; i < sandpile.height; i++){
        if (sandpile.matrich[i][sandpile.width - 1] >= 4) {
            right = 1;
        } 
        if (sandpile.matrich[i][0] >= 4) {
            left = 1;
            diff_width = 1;
        }
    }

    uint64_t** copy_arr = new uint64_t*[sandpile.height + up + down];
    for (int i = 0; i < (sandpile.height + up + down); ++i) {
        copy_arr[i] = new uint64_t[sandpile.width + right + left];
        std::memset(copy_arr[i], 0, (sandpile.width + right + left) * 8);
    }
    
    for (int i = 0; i < sandpile.height; ++i) {
        for (int j = 0; j < sandpile.width; ++j) {
            copy_arr[i + diff_height][j + diff_width] = sandpile.matrich[i][j];
        }
    }

    for (int i = 0; i < sandpile.height; ++i) {
        delete[] sandpile.matrich[i];
    }
    
    delete[] sandpile.matrich;

    sandpile.height = sandpile.height + up + down;
    sandpile.width = sandpile.width + right + left;

    sandpile.matrich = copy_arr;
}
