#pragma ONCE

#include "vec3.hpp"
#include <cmath>

struct Camera
{
    Vec3f position;
    Vec3f orientation;
    Vec3f vue_x, vue_y;
    int FOV;
    float aspect_ratio;

    Camera()
    {
        FOV = 45;
        aspect_ratio = 3 / 2;
        position = Vec3f(0,0,0);
        orientation = Vec3f(0, 0, -1);
        vue_x = Vec3f(-1, 0, 0);
        vue_y = Vec3f(0, 1, 0);
    }

    Camera(float ar)
    {
        FOV = 45;
        aspect_ratio = ar;
        position = Vec3f(0,0,0);
        orientation = Vec3f(0, 0, -1);
        vue_x = Vec3f(-1, 0, 0);
        vue_y = Vec3f(0, 1, 0);
    }

    Vec3f position3D(Image pic, int i, int j)
    {
        const float center_i = pic.get_width() /2.0;
        const float center_j = pic.get_height() /2.0;
        const float dec_x = i - center_i;
        const float dec_y = j - center_j;
        Vec3f aim = position + orientation; // start from center and move along the axis
        //aim += dec_x / center_i * FOV * vue_x + dec_y / center_j * FOV * vue_y;

        aim = aim + dec_x/center_i*vue_x + dec_y/center_j*vue_y - position;
        return aim;
    }
};