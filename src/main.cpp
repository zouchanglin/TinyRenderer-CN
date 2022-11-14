#include "lib/tgaimage.h"
#include "lib/geometry.h"
#include <cmath>
#include <iostream>
#include "lib/model.h"
#include <vector>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);

const int width  = 300;
const int height = 300;

void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color) {
    int x0 = t0.x;
    int y0 = t0.y;
    int x1 = t1.x;
    int y1 = t1.y;
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

void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
//    line(t0, t1, image, color);
//    line(t1, t2, image, color);
//    line(t2, t0, image, color);

    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);

    line(t0, t1, image, green);
    line(t1, t2, image, green);
    line(t2, t0, image, red);
}

void triangle3(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta  = (float)(y - t0.y) / segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t0 + (t1 - t0) * beta;

        // 绘制水平扫线
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }

    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height =  t2.y - t1.y + 1;
        float alpha = (float)(y - t0.y) / total_height;
        float beta  = (float)(y - t1.y) / segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = t1 + (t2 - t1) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}


void triangle4(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    if (t0.y == t1.y && t0.y == t2.y) return; // 不用关心非三角形
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, t0.y + i, color);
        }
    }
}

double triangle_area_2d(const Vec2i &a, const Vec2i &b, const Vec2i &c) {
    return .5 * ((b.y - a.y) * (b.x + a.x) + (c.y - b.y) * (c.x + b.x) + (a.y - c.y) * (a.x + c.x));
}

Vec3f barycentric(Vec2i A, Vec2i B, Vec2i C, Vec2i P) {
    double total_area = triangle_area_2d(A, B, C);
    double u = triangle_area_2d(P, B, C);
    double v = triangle_area_2d(P, C, A);
    double w = triangle_area_2d(P, A, B);
    return Vec3f(u/total_area, v/total_area, w/total_area);
}

void triangle(Vec2i *pts, TGAImage &image, TGAColor color) {
    Vec2i bboxmin(image.get_width()-1,  image.get_height()-1);
    Vec2i bboxmax(0, 0);
    Vec2i clamp(image.get_width()-1, image.get_height()-1);
    for (int i=0; i<3; i++) {
        bboxmin.x = std::max(0, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, pts[i].y));

        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
    }
    Vec2i P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen  = barycentric(pts[0],pts[1],pts[2], P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            image.set(P.x, P.y, color);
        }
    }
}

int main(int argc, char** argv) {
    TGAImage frame(200, 200, TGAImage::RGB);
    Vec2i pts[3] = {Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160)};
    triangle(pts, frame, red);
    frame.flip_vertically(); // to place the origin in the bottom left corner of the image
    frame.write_tga_file("output.tga");
    return 0;
}

//int main2(int argc, char* argv[]){
//    TGAImage image(width, height, TGAImage::RGB);
//    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
//    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
//    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
//    triangle(t0[0], t0[1], t0[2], image, red);
//    triangle(t1[0], t1[1], t1[2], image, white);
//    triangle(t2[0], t2[1], t2[2], image, green);
//
//    // 垂直翻转，因为我们习惯性以左下角作为坐标原点
//    image.flip_vertically();
//    image.write_tga_file("output.tga");
//
//    return 0;
//}
