#ifndef RAY_H
#define RAY_H

#include "st.h"
#include "Camera.h"

class Ray
{
  public:
    Ray();
    Ray(Camera &camera, const STVector3 &p);
    Ray(const STVector3 &source, const STVector3 &dest);
    STVector3 getEye();
    STVector3 getDirection();
    STVector3 getPointAt(float t);
    Ray getTransformedRay(const STTransform4 &transform);
  private:
    STVector3 eye, direction;
};

#endif
