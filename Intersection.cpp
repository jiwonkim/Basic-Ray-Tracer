#include "Intersection.h"


Intersection::Intersection(float _t, const STVector3 &_point, const STVector3 &_normal, const STVector3 &_direction, SceneObject *_object) {
  t = _t;
  point = _point;
  normal = _normal;
  object = _object;
  direction = _direction;
}

STVector3 Intersection::getNormal() {
  return normal;
}

STVector3 Intersection::getDirection() {
  return direction;
}

STVector3 Intersection::getIntersectionPoint() {
  return point;
}

float Intersection::getTValue() {
  return t;
}

SceneObject *Intersection::getObject() {
  return object;
}
