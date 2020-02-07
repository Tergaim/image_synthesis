#include "image.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "ray.hpp"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
//#define raysPerPixel 1

void printProgress(double percentage)
{
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}


bool rayTriangleIntersect(Ray ray, Vec3f p0, Vec3f p1, Vec3f p2, float result[4])
{
    const Vec3f e0 = p1 - p0;
    const Vec3f e1 = p2 - p0;
    const Vec3f n = normalize(cross(e0, e1));
    const Vec3f q = cross(ray.direction(), e1);
    float a = dot(e0, q);

    if (dot(n, ray.direction()) >= 0 || (a < 1e-3 && a > -1e-3))
        return false;
    const Vec3f s = (ray.origin() - p0) / a;
    const Vec3f r = cross(s, e0);
    result[0] = dot(s, q);
    result[1] = dot(r, ray.direction());
    result[2] = 1 - result[0] - result[1];
    result[3] = dot(e1, r);

    if (result[0] < 0 || result[1] < 0 || result[2] < 0 || result[3] < 0)
        return false;
    return true;
}


bool rayTrace(Image &picture, const Scene &scene, Ray ray, int &indexMesh, int &indexTriangle, float result_glob[4])
{
    float d = 1e8;
    
    for (int i = 0; i < scene.meshes.size(); i++)
    {
        Mesh m = scene.meshes[i];
        for (int j=0; j<m.indexedTriangles().size(); j++)
        {
            Vec3i t = m.indexedTriangles()[j];
            float result[4];
            bool intersect = ray.intersectTriangle(m.vertexPositions()[t[0]], m.vertexPositions()[t[1]], m.vertexPositions()[t[2]], result);
            if (intersect && result[3] < d)
            {
                d = result[3];
                for(int k = 0; k<4; k++)   
                    result_glob[k] = result[k];
                indexMesh = i;
                indexTriangle = j;
            }
        }
    }
    if(d < 1e7){
      return true;
    }
    return false;
}

Vec3f shade(const Scene& scene, const Vec3f& n, const Vec3f& wi, const int mIndex){
    return scene.meshes[mIndex].material().evaluateColorResponse(n, wi, Vec3f(0,0,0));
}

void render(Image &img, const Scene &scene)
{
    // if (img.get_width() == scene.camera.aspect_ratio * img.get_height())
    // {
        vector<Mesh> M = scene.meshes;
        for (size_t x = 0; x < img.get_width(); ++x)
        {
            printProgress(float(x) / img.get_width());
            //cout << "Processing column " << x << endl;
            for (size_t y = 0; y < img.get_height(); ++y)
            {
                Vec3f pixelPosition = scene.camera.position3D_pixels(x, y);
                Vec3f cameraPosition = scene.camera.position;
                int raysPerPixel = 1;
                float pixelWidth = (scene.camera.position3D_pixels(1, 1) - scene.camera.position3D_pixels(0, 0))[0];
                float pixelHeight = (scene.camera.position3D_pixels(1, 1) - scene.camera.position3D_pixels(0, 0))[1];
                vector<Vec3f> colorResponses;
                for (int i = 0; i < raysPerPixel; ++i)
                {
                    Ray ray = Ray(cameraPosition, normalize(pixelPosition + Vec3f((float)rand() / (float)RAND_MAX * pixelWidth, (float)rand() / (float)RAND_MAX * pixelHeight, 0.f) - cameraPosition));
                    float result[4];
                    int hitMesh, hitTriangle;
                    bool intersect = rayTrace(img, scene, ray, hitMesh, hitTriangle, result);
                    
                    if (intersect && result[3] > 0.f)
                    {
                        Vec3f n0 = M[hitMesh].vertexNormals()[M[hitMesh].indexedTriangles()[hitTriangle][0]];
                        Vec3f n1 = M[hitMesh].vertexNormals()[M[hitMesh].indexedTriangles()[hitTriangle][1]];
                        Vec3f n2 = M[hitMesh].vertexNormals()[M[hitMesh].indexedTriangles()[hitTriangle][2]];
                        Vec3f p0 = M[hitMesh].vertexPositions()[M[hitMesh].indexedTriangles()[hitTriangle][0]];
                        Vec3f p1 = M[hitMesh].vertexPositions()[M[hitMesh].indexedTriangles()[hitTriangle][1]];
                        Vec3f p2 = M[hitMesh].vertexPositions()[M[hitMesh].indexedTriangles()[hitTriangle][2]];
                        Vec3f hitNormal = normalize(result[0] * n0 + result[1] * n1 + result[2] * n2);
                        Vec3f hitLocation = normalize(result[0] * p0 + result[1] * p1 + result[2] * p2);
                        Ray s_ray = Ray(hitLocation, normalize(scene.lightsource.m_position - hitLocation));
                        float s_result[4];
                        int s_hitMesh, s_hitTriangle;
                        bool s_intersect = rayTrace(img, scene, s_ray, s_hitMesh, s_hitTriangle, s_result);
                        if (s_intersect && s_result[3] > 0.f)
                        {
                            colorResponses.push_back(Vec3f(0.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            colorResponses.push_back(shade(scene, hitNormal, normalize(scene.lightsource.m_position - hitLocation), hitMesh));
                        }

                    }
                    else
                    {
                        colorResponses.push_back(img(x, y));
                    }
                }
                Vec3f colorResponse = Vec3f();
                for (auto &c : colorResponses)
                    colorResponse += c;
                colorResponse /= colorResponses.size();
                img.set(x, y, colorResponse);
            }
        }
    // }
    // else
    // {
    //     std::cout << "rayTrace failure: image size differs from camera aspect ratio" << std::endl;
    // }
}