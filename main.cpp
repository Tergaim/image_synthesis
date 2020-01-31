#include "src/image.hpp"
#include "src/scene.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

void rayTrace(Image &picture, Scene scene);
bool rayTriangleIntersect(Vec3f source, Vec3f direction, Vec3f p0, Vec3f p1, Vec3f p2, float result[4]);

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
    Scene scene = Scene(w, h);
    scene.add_triangle(Vec3f(0, 0, -1), Vec3f(1, 0, -1), Vec3f(0, 1, -1));

    rayTrace(picture, scene);
    std::cout << "Saving at location " << filename << std::endl;
    picture.savePPM(filename);

    return 0;
}

void rayTrace(Image &picture, Scene scene)
{
    const Vec3f c = scene.camera.position;
    for (int i = 0; i < picture.get_width(); i++)
    {
        printProgress(float(i+1)/picture.get_width());
        for (int j = 0; j < picture.get_height(); j++)
        {
            float d = 1e8;
            const Vec3f p = scene.camera.position3D(picture, i, j);
            for (Vec3i t : scene.triangles)
            {
                float result[4];
                Vec3f orientation = normalize(p - c);
                bool intersect = rayTriangleIntersect(c, orientation, scene.vertices[t[0]], scene.vertices[t[1]], scene.vertices[t[2]], result);
                if (intersect && result[3] < d)
                {
                    d = result[3];
                    picture.set(i, j, Vec3f(1.f, 1.f, 1.f));
                }
            }
        }
    }
    std::cout<<std::endl;
}

bool rayTriangleIntersect(Vec3f source, Vec3f direction, Vec3f p0, Vec3f p1, Vec3f p2, float result[4])
{
    const Vec3f e0 = p1 - p0;
    const Vec3f e1 = p2 - p0;
    const Vec3f n = normalize(cross(e0, e1));
    const Vec3f q = cross(direction, e1);
    float a = dot(e0, q);

    if (dot(n, direction) >= 0 || (a < 1e-3 && a > -1e-3))
        return false;
    const Vec3f s = (source - p0) / a;
    const Vec3f r = cross(s, e0);
    result[0] = dot(s, q);
    result[1] = dot(r, direction);
    result[2] = 1 - result[0] - result[1];
    result[3] = dot(e1, r);

    if (result[0] < 0 || result[1] < 0 || result[2] < 0 || result[3] < 0)
        return false;
    return true;
}
