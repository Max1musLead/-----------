#include "bmp/bmp.h"
#include "parser/parser.h"
#include "sandpile_model/sandpile.h"

int main(int argc, char** argv) {
    Sandpile sandpile;
    Image image_res;
    Input input = ParseArgumetns(argc, argv);
    char* name;
    int value;

    InitSandpileFromTsv(input.filename, sandpile);

    if (input.freq == 0 && input.iter == 0) {
        SandpileAllСrumble(sandpile);
    } else {
        if (input.iter == 0) {
            while (!input.is_stable){
                char* name;
                input.is_stable = SandpileСrumble(sandpile);
                sprintf(name, "%d", input.cnt_freq);
                if (input.cnt_freq % input.freq == 0) {
                    image_res = CreateBmp(sandpile.width, sandpile.height, sandpile.matrich);
                    image_res = DoBMPPixelField(image_res.matrich, image_res.width, image_res.height, image_res);
                    SaveBMP(input.output, name, image_res);
                }
                input.cnt_freq++;
            }
        } else {
            while (input.cnt_iter != input.iter && !input.is_stable) {
                input.is_stable = SandpileСrumble(sandpile);
                sprintf(name, "%d", input.cnt_iter);
                if (input.freq != 0 && (input.cnt_freq % input.freq == 0)) {
                    image_res = CreateBmp(sandpile.width, sandpile.height, sandpile.matrich);
                    image_res = DoBMPPixelField(image_res.matrich, image_res.width, image_res.height, image_res);
                    SaveBMP(input.output, name, image_res);
                }
                input.cnt_iter++;
                input.cnt_freq++;
            }
        }
    }

    strcpy(name, "Result");
    image_res = CreateBmp(sandpile.width, sandpile.height, sandpile.matrich);
    image_res = DoBMPPixelField(image_res.matrich, image_res.width, image_res.height, image_res);
    SaveBMP(input.output, name, image_res);

    strcpy(name, "Result2");
    image_res(3, 3) = 4;
    image_res = CreateBmp(image_res.width, image_res.height, image_res.matrich);
    image_res = DoBMPPixelField(image_res.matrich, image_res.width, image_res.height, image_res);
    SaveBMP(input.output, name, image_res);

    strcpy(name, "Result3");
    image_res = LoadFromDisk(input.load);
    SaveBMP(input.output, name, image_res);
}
