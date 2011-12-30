#ifndef SPHERE_H
#define SPHERE_H

#include "st.h"
#include "Shape.h"

class Sphere : public Shape
{
  public:
    Sphere(const STPoint3 &_center, const float _radius);
    float intersects(Ray ray);
    STVector3 getNormalAt(STVector3 point, STVector3 direction);
    std::string toString();
  private:
    STVector3 center;
    float radius;
};

#endif
