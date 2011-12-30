#include "Ray.h"

Ray::Ray() {
  eye = STVector3(0,0,0);
  direction = STVector3(0,0,0);
}

Ray::Ray(Camera &camera, const STVector3 &p) {
  eye = camera.getEye();
  direction = p-eye;
}

Ray::Ray(const STVector3 &source, const STVector3 &dest) {
  eye = source;
  direction = dest-source;
}

STVector3 Ray::getEye() {
  return eye;
}

STVector3 Ray::getDirection() {
  return direction;
}

STVector3 Ray::getPointAt(float t) {
  return (eye + t*direction);
}

Ray::Ray Ray::getTransformedRay(const STTransform4 &transform) {
  STVector3 new_source = STVector3(transform*STPoint3(eye));
  STVector3 new_dest = STVector3(transform*STPoint3(eye+direction));
  return Ray(new_source, new_dest);
}
