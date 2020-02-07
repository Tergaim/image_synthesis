#pragma once

#include<cmath>
#include "vec3.hpp"

#define PI 3.14159

class Material{
public:
  float alpha2;
  Vec3f albedo;
  float Kd;
  Material(float alpha = 1, Vec3f albedo = Vec3f(1.0f,1.0f,1.0f), float Kd = 3.0f) : alpha2(alpha*alpha), albedo(albedo), Kd(Kd){};

  Vec3f evaluateColorResponse (const Vec3f & normal, const Vec3f & wi) const{
      return float(fmax(dot(normal, wi),0.0f))*Kd/3.14f*albedo;
  }

  

  Vec3f evaluateColorResponse (const Vec3f & normal, const Vec3f & wi, const Vec3f &wo) const{
      const Vec3f wh = normalize(wi + wo);
      const float detni = dot(normal,wi);
      const float detno = dot(normal,wo);
      const float detnh = dot(normal,wh);
      
      const float denom_tmp_D = (1+(alpha2-1)*detnh*detnh);
      const float denom_D = 1/(PI*denom_tmp_D*denom_tmp_D);


      const float denom_Gi = 1/(detni + std::sqrt(alpha2 + (1-alpha2)*detni*detni));
      const float denom_Go = 1/(detno + std::sqrt(alpha2 + (1-alpha2)*detno*detno));

      const float G = 4*detni*detno*denom_Gi*denom_Go;




      return float(fmax(dot(normal, wi),0.0f))*Kd/3.14f*albedo;
  }
};
