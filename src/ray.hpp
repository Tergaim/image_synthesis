#pragma once

#include <iostream>
#include "vec3.hpp"

class Ray{
private:
  Vec3f m_origin;
  Vec3f m_direction;
  Vec3f m_hit;

public:
  inline Ray(const Vec3f& origin, const Vec3f& direction) : m_origin(origin), m_direction(normalize(direction)) {};
  inline ~Ray () {};

  inline const Vec3f& origin() const { return m_origin; };

  inline const Vec3f& direction() const { return m_direction; };

  // bool intersectTriangle(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, float& u, float& v, float& d){
  //   float EPSILON=0.000001f;
  //   Vec3f e0 = p1-p0;
  //   Vec3f e1 = p2-p0;
  //   Vec3f n = normalize(cross(e0, e1));
  //   Vec3f q = cross(m_direction,e1);
  //   float a = dot(e0,q);
  //   if(fabs(a) < EPSILON || dot(n,m_direction)>=0){
  //     return false;
  //   }
  //   Vec3f s = (m_origin - p0)/a;
  //   Vec3f r = cross(s,e0);
  //   u = dot(s,q);
  //   v = dot(r,m_direction);
  //   if(u<0 || v<0 || u+v>1){
  //     return false;
  //   }
  //   d = dot(e1,r);
  //   if(d>=0){
  //     return true;
  //   }
  //   return false;
  // }

  bool intersectTriangle(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, float result[4]){
    float EPSILON=0.000001f;
    Vec3f e0 = p1-p0;
    Vec3f e1 = p2-p0;
    Vec3f e2 = normalize(cross(e0, e1));
    float det = dot(m_direction, e2);
    Vec3f a = e2/length(cross(e0, e1));
    if(fabs(det) < EPSILON || dot(m_direction,e2)>=0){
      return false;
    }
    Vec3f ovec = m_origin - p0;
    result[3] = -dot(ovec,e2)/det;
    Vec3f hit = m_origin + result[3]*m_direction;
    //u = dot(cross(p2-p1,hit-p1), a);
    //v = dot(cross(p0-p2,hit-p2), a);
    result[1] = dot(e2, cross(hit-p0,e1))/length(cross(e0,e1));
    result[0] = 1.f-result[1]+dot(e2, cross(hit-p0,e0))/length(cross(e1,e0));
    result[2] = 1-result[0]-result[1];
    if(result[0]<0 || result[1]<0 || result[0]+result[1]>1){
      return false;
    }
    if(result[3]>=0){
      return true;
    }
    return false;
  }
};
