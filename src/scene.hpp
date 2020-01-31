#pragma ONCE
#include <vector>
#include "vec3.hpp"
#include "camera.hpp"

struct Scene {

    std::vector<Vec3f> vertices;
    std::vector<Vec3i> triangles;
    Camera camera;

    Scene(){
        camera = Camera();
    }

    Scene(int w, int h){
        camera = Camera(float(w)/h);
    }

    void add_triangle(Vec3f p1, Vec3f p2, Vec3f p3) {
        int l = vertices.size();
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        triangles.push_back(Vec3i(l, l+1, l+2));
    }
};