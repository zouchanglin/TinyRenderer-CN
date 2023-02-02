#include <vector>
#include <cstdlib>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const int width  = 800;
const int height = 800;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) {
        return {1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z};
    }
    return {-1,1,1};
}

float min(float a, float b){
    return a < b ? a:b;
}

float max(float a, float b){
    return a > b ? a:b;
}

void triangle(Vec3f *pts, float *zBuffer, TGAImage &image, TGAColor color, TGAImage &zimage) {
    Vec2f bBoxMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bBoxMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bBoxMin[j] = max(0.f, min(bBoxMin[j], pts[i][j]));
            bBoxMax[j] = min(clamp[j], max(bBoxMax[j], pts[i][j]));
        }
    }
    Vec3f P;
    for (P.x = bBoxMin.x; P.x <= bBoxMax.x; P.x++) {
        for (P.y = bBoxMin.y; P.y <= bBoxMax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            P.z = 0;
            for (int i = 0; i < 3; i++) P.z += pts[i][2]*bc_screen[i];
            if (zBuffer[int(P.x + P.y * width)] < P.z) {
                zBuffer[int(P.x + P.y * width)] = P.z;
                image.set(P.x, P.y, color);
                // 顺便绘制一下深度图
                zimage.set(P.x, P.y, white * P.z);
            }
        }
    }
}

Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x + 1.) * width/2. + .5), int((v.y + 1.) * height/2. + .5), v.z);
}

int main(int argc, char* argv[]){
    Model *model;
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../obj/african_head.obj");
    }
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zImage(width, height, TGAImage::RGBA);

    // init z-buffer
    auto *zBuffer = new float[width * height];
    for (int i=width * height; i--; zBuffer[i] = -std::numeric_limits<float>::max());

    Vec3f light_dir(0, 0, -1);
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec3f pts[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++) {
            pts[j] = world2screen(model->vert(face[j]));
            world_coords[j] = model->vert(face[j]);
        }

        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
        n.normalize();

        int intensity = (int)(n * light_dir * 255);
//        triangle(pts, zBuffer, image, TGAColor(rand()%255, rand()%255, rand()%255, 255), zImage);
        triangle(pts, zBuffer, image, TGAColor(intensity, intensity, intensity, 255), zImage);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");

    zImage.flip_vertically();
    zImage.write_tga_file("z-output.tga");

    delete model;
    return 0;
}
