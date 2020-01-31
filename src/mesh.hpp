#pragma once

#include <vector>
#include "vec3.hpp"
#include "material.hpp"

using namespace std;

class Mesh{
private:
  vector<Vec3f> m_vertexPositions;
  vector<Vec3f> m_vertexNormals;
  vector<Vec3i> m_indexedTriangles;
  Material m_material;

public:
  inline Mesh(){};
  virtual ~Mesh(){};

  inline vector<Vec3f> & vertexPositions() { return m_vertexPositions; };
  inline const vector<Vec3f> & vertexPositions() const { return m_vertexPositions; };

  inline vector<Vec3f> & vertexNormals() { return m_vertexNormals; };
  inline const vector<Vec3f> & vertexNormals() const { return m_vertexNormals; };

  inline vector<Vec3i> & indexedTriangles() { return m_indexedTriangles; };
  inline const vector<Vec3i> & indexedTriangles() const { return m_indexedTriangles; };

  inline Material & material() { return m_material; };
  inline const Material & material() const { return m_material; };

  void compute_normals(){
    m_vertexNormals.resize(m_vertexPositions.size());
    for(int k=0; k<m_indexedTriangles.size(); ++k){
      Vec3i triangle = m_indexedTriangles[k];
      Vec3f e0 = m_vertexPositions[triangle[1]] - m_vertexPositions[triangle[0]];
      Vec3f e1 = m_vertexPositions[triangle[2]] - m_vertexPositions[triangle[0]];
      Vec3f n = normalize(cross(e0, e1));
      m_vertexNormals[triangle[0]] += n;
      m_vertexNormals[triangle[1]] += n;
      m_vertexNormals[triangle[2]] += n;
    }
    for(int i=0; i<m_vertexPositions.size(); ++i){
      m_vertexNormals[i] = normalize(m_vertexNormals[i]);
    }
  }
};
