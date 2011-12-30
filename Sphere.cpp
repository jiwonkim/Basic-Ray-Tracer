#include "Sphere.h"

Sphere::Sphere(const STPoint3 &_center, const float _radius) {
  center = STVector3(_center);
  radius = _radius;
}

float Sphere::intersects(Ray ray) {
  STVector3 E = ray.getEye();
  STVector3 D = ray.getDirection();
  STVector3 E_C = E-STVector3(center);
  
  // Solves quadratic equation for t
  float b = 2*STVector3::Dot(D,E_C);
  float a = STVector3::Dot(D,D);
  float c = STVector3::Dot(E_C,E_C) - radius*radius;

  float discriminant = b*b-4*a*c;
  
  // imaginary root - return negative
  if(discriminant < 0) {
    return -1;
  }
  // if one intersection, will return the one t value.
  // for two intersections, will return the t value closer to the viewer
  float t = (-b - sqrt(discriminant))/(2.*a);
  if(t>0) return t;
  return (-b + sqrt(discriminant))/(2.*a);
}

STVector3 Sphere::getNormalAt(STVector3 point, STVector3 direction) {
  return (point-center)/(point-center).Length();
}

std::string Sphere::toString() {
  return "Sphere object";
}
