#include "lib/tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char* argv[]){
    TGAImage image(100, 100, TGAImage::RGB);
    for (int i = 0; i < 100; ++i) {
        image.set(80, i, white);
    }

    for (int i = 0; i < 100; ++i) {
        image.set(i, 10, red);
    }

    image.set(1,1,red);

    // 垂直翻转，因为我们习惯性以左下角作为坐标原点
    image.flip_vertically();

    image.write_tga_file("output.tga");
    return 0;
}