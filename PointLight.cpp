#include "PointLight.h"
#include <stdio.h>

PointLight::PointLight(const STPoint3 &loc, const STColor3f &col) : Light(col) {
  location = STVector3(loc);
}

STVector3 PointLight::getDirectionTo(STVector3 &point) {
  return location-point;
}

STVector3 PointLight::getLocation(STVector3 &point) {
  return location;
}
