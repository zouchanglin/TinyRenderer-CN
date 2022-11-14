#include "lib/tgaimage.h"
#include <cmath>
#include <iostream>
#include "lib/model.h"
#include <vector>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

const int width  = 800;
const int height = 800;

#define FIFTH_ATTEMPT

#ifdef FIRST_ATTEMPT
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for (float t = 0.; t < 1.; t += 0.01) {
        int x = x0 + (x1 - x0) * t;
        int y = y0 + (y1 - y0) * t;
        image.set(x, y, color);
    }
}
#endif

#ifdef SECOND_ATTEMPT
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    for (int x = x0; x <= x1; x++) {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        image.set(x, y, color);
    }
}
#endif

#ifdef THIRD_ATTEMPT
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) { // 如果线段很陡，我们转置图像
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // 确保线段是从左往右绘制
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x = x0; x <= x1; x++) {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        if (steep) {
            image.set(y, x, color); // 如果图像已经转置，则先还原再绘制
        } else {
            image.set(x, y, color);
        }
    }
}
#endif


#ifdef FOURTH_ATTEMPT
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) { // 如果线段很陡，我们转置图像
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // 确保线段是从左往右绘制
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // 求斜率
    float slope = abs((float)(y1 - y0)/(float)(x1 - x0));
    float step = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color); // 如果图像已经转置，则先还原再绘制
        } else {
            image.set(x, y, color);
        }
        step += slope;
        if(step > 0.5){
            y += (y1 > y0 ? 1:-1); // 直线的方向影响
            step -= 1;
        }
    }
}

#endif


#ifdef FIFTH_ATTEMPT
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1)) { // 如果线段很陡，我们转置图像
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { // 确保线段是从左往右绘制
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int slope = 2 * dy;
    int step = 0;

    int y = y0;

//    for (int x = x0; x <= x1; x++) {
//        if (steep) {
//            image.set(y, x, color); // 如果图像已经转置，则先还原再绘制
//        } else {
//            image.set(x, y, color);
//        }
//        step += slope;
//        if(step > dx){
//            y += (y1 > y0 ? 1:-1); // 直线的方向影响
//            step -= dx * 2;
//        }
//    }

    // 消除循环中的分支指令是一个令人印象深刻的加速过程！
    // https://github.com/ssloy/tinyrenderer/issues/28
    const int yinCr = (y1 > y0 ? 1 : -1);
    if(steep) {
        for(int x = x0; x<=x1; ++x) {
            image.set(y, x, color);
            step += slope;
            if(step > dx) {
                y += yinCr;
                step -= dx * 2;
            }
        }
    } else {
        for(int x = x0; x<=x1; ++x) {
            image.set(x, y, color);
            step += slope;
            if(step > dx) {
                y += yinCr;
                step -= dx * 2;
            }
        }
    }
}

#endif

int main_back(int argc, char* argv[]){
    TGAImage image(100, 100, TGAImage::RGB);

    line(0,0, 50, 20, image, white);
//    line(0,0, 50, 100, image, red);
//    line(13, 20, 80, 40, image, white);
//    line(20, 13, 40, 80, image, red);
//    line(70, 30, 13, 20, image, red);
//    line(13, 80, 80, 20, image, red);
//    line(20, 30, 50, 30, image, white);
//    line(25, 0, 25, 80, image, white);

//    Model *model = nullptr;
//
//    if (2==argc) {
//        model = new Model(argv[1]);
//    } else {
//        model = new Model("../obj/african_head.obj");
//    }
//    TGAImage image(width, height, TGAImage::RGB);
//
//    for (int i = 0; i < model->nfaces(); ++i) {
//        std::vector<int> face = model->face(i);
//        for (int j = 0; j < 3; ++j) {
//            Vec3f v0 = model->vert(face[j]);
//            Vec3f v1 = model->vert(face[(j + 1) % 3]);
//            int x0 = (v0.x + 1.) * width / 2.;
//            int y0 = (v0.y + 1.) * height / 2.;
//            int x1 = (v1.x + 1.) * width / 2.;
//            int y1 = (v1.y + 1.) * height / 2.;
//            line(x0, y0, x1, y1, image, white);
//        }
//    }

    // 垂直翻转，因为我们习惯性以左下角作为坐标原点
    image.flip_vertically();
    image.write_tga_file("output.tga");

//    delete model;
    return 0;
}
