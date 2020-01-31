#pragma once

#include "vec3.hpp"

class LightSource{
public:
  Vec3f m_position;
  Vec3f m_color;
  float m_intensity;
  LightSource(Vec3f position=Vec3f(), Vec3f color=Vec3f(1.0f,1.0f,1.0f), float intensity=1.0f) : m_position(position), m_color(color), m_intensity(intensity){};
};
