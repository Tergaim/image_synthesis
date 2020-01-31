#pragma once
#include <fstream>
#include <vector>
#include <sstream>
#include "vec3.hpp"
#include "camera.hpp"
#include "lightSource.hpp"
#include "mesh.hpp"

using std::vector;

struct Scene
{
    vector<Mesh> meshes;
    Camera camera;
    LightSource lightsource;

    inline Scene(){};
    virtual ~Scene(){};

    void addTriangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2)
    {
        Mesh mesh = Mesh();
        float nT = mesh.vertexPositions().size();
        mesh.vertexPositions().push_back(p0);
        mesh.vertexPositions().push_back(p1);
        mesh.vertexPositions().push_back(p2);
        mesh.indexedTriangles().push_back(Vec3i(nT, nT + 1, nT + 2));
        meshes.push_back(mesh);
    }

    void addTriangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2, Mesh &mesh)
    {
        float nT = mesh.vertexPositions().size();
        mesh.vertexPositions().push_back(p0);
        mesh.vertexPositions().push_back(p1);
        mesh.vertexPositions().push_back(p2);
        mesh.indexedTriangles().push_back(Vec3i(nT, nT + 1, nT + 2));
    }

    void loadOFF(const std::string &filepath)
    {
        std::ifstream input(filepath);
        Mesh mesh;
        int nVertices, nFaces;
        //read first line to check document format
        std::string line;
        std::getline(input, line);
        if (line.substr(0, 3).compare("OFF") != 0)
        {
            cout << "file not in OFF format !" << endl;
        }
        else
        {
            //retrieve number of vertices and faces
            std::getline(input, line);
            std::istringstream iss(line);
            iss >> nVertices >> nFaces;
            cout << "nVertices : " << nVertices << endl
                 << "nFaces : " << nFaces << endl;
        }
        //add vertices
        for (int i = 0; i < nVertices; ++i)
        {
            std::getline(input, line);
            std::istringstream iss(line);
            Vec3f vertex;
            iss >> vertex[0] >> vertex[1] >> vertex[2];
            mesh.vertexPositions().push_back(vertex);
            mesh.vertexNormals().push_back(Vec3f());
        }
        for (int k = 0; k < nFaces; ++k)
        {
            std::getline(input, line);
            std::istringstream iss(line);
            Vec3i triangle;
            int faceDegree;
            iss >> faceDegree;
            if (faceDegree == 3)
            {
                iss >> triangle[0] >> triangle[1] >> triangle[2];
            }
            else
            {
                cout << "Non triangular face detected and ignored" << endl;
            }
            mesh.indexedTriangles().push_back(triangle);
            Vec3f e0 = mesh.vertexPositions()[triangle[1]] - mesh.vertexPositions()[triangle[0]];
            Vec3f e1 = mesh.vertexPositions()[triangle[2]] - mesh.vertexPositions()[triangle[0]];
            Vec3f n = normalize(cross(e0, e1));
            mesh.vertexNormals()[triangle[0]] += n;
            mesh.vertexNormals()[triangle[1]] += n;
            mesh.vertexNormals()[triangle[2]] += n;
        }
        for (int i = 0; i < nVertices; ++i)
        {
            mesh.vertexNormals()[i] = normalize(mesh.vertexNormals()[i]);
        }
        meshes.push_back(mesh);
    }
};
