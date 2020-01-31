#pragma once

#include<cmath>
#include "vec3.hpp"

class Material{
public:
  Vec3f m_albedo;
  float m_Kd;
  Material(Vec3f albedo = Vec3f(1.0f,1.0f,1.0f), float Kd = 3.0f) : m_albedo(albedo), m_Kd(Kd){};

  Vec3f evaluateColorResponse (const Vec3f & normal, const Vec3f & wi) const{
    return float(fmax(dot(normal, wi),0.0f))*m_Kd/3.14f*m_albedo;
  }
};
