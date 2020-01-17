#pragma ONCE

#include "src/vec3.hpp"

struct Camera{
    Vec3f position;
    Vec3f orientation;
    int FOV;
    float aspect_ratio;

    Camera(){
        FOV = 45;
        aspect_ratio = 3/2;
        position = Vec3f();
        orientation = Vec3f{0,0,-1};
    }    

    Camera(float ar){
        FOV = 45;
        aspect_ratio = ar;
        position = Vec3f();
        orientation = Vec3f{0,0,-1};
    } 
};