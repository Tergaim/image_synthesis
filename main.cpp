#include "src/image.hpp"
#include "src/camera.hpp"
#include "src/scene.hpp"
#include "src/ray.hpp"
#include "src/raytracer.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[])
{
    int h, w;
    char *filename;
    for (int i = 1; i < argc; i += 2)
    {
        const std::string s = std::string(argv[i]);
        if (s == "-height")
        {
            h = std::atoi(argv[i + 1]);
        }
        else if (s == "-width")
        {
            w = std::atoi(argv[i + 1]);
        }
        else if (s == "-output")
        {
            filename = argv[i + 1];
        }
        else
        {
            std::cout << "Unknown argument: " << argv[i] << " at index " << i << std::endl;
        }
    }
    // Create background image
    Image picture = Image(w, h, Vec3f(1, 0, 1));
    picture.fillBackground(Vec3f(0, 0, 1));

    // generate one triangle
    Scene scene;
    scene.camera = Camera(Vec3f(0.0f, 0.f, 10.7f), Vec3f(0.f,0.f,-1.f));
    scene.camera.position3D(picture);
    scene.lightsource = LightSource(Vec3f(2.0f,4.0f,2.0f));
    //scene.addTriangle(Vec3f(0.f, 0.f, -1.f), Vec32(0.f, 1.f, -2.f), Vec3f(1.f, 0.f, -1.f));
    //scene.addTriangle(Vec3f(0.f, 0.f, -2.f), Vec3f(-1.f, 0.f, -2.f), Vec3f(0.f, -1.f, -2.f));
    scene.loadOFF("data/example.off");

    Mesh ground = Mesh();
    ground.vertexPositions().push_back(Vec3f(-20.f, -0.5f, -20.f));
    ground.vertexPositions().push_back(Vec3f(20.f, -0.5f, -20.f));
    ground.vertexPositions().push_back(Vec3f(20.f, -0.5f, 20.f));
    ground.vertexPositions().push_back(Vec3f(-20.f, -0.5f, 20.f));
    ground.indexedTriangles().push_back(Vec3i(0,2,1));
    ground.indexedTriangles().push_back(Vec3i(0,3,2));
    ground.compute_normals();
    scene.meshes.push_back(ground);

    render(picture, scene);
    std::cout << "Saving at location " << filename << std::endl;
    picture.savePPM(filename);

    return 0;
}