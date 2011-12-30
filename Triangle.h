#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "st.h"

class Triangle : public Shape
{
  public:
    Triangle(const STPoint3 &_v0, const STPoint3 &_v1, const STPoint3 &_v2);
    float intersects(Ray ray);
    STVector3 getNormalAt(STVector3 point, STVector3 direction);
    std::string toString();

  private:
    STPoint3 v0, v1, v2;   
};

#endif
