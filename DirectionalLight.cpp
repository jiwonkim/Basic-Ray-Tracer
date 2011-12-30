#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const STVector3 &dir, const STColor3f& col) : Light(col) {
  direction = dir;
}

STVector3 DirectionalLight::getDirectionTo(STVector3 &point) {
  return -direction;
}

STVector3 DirectionalLight::getLocation(STVector3 &point) {
  return point-direction;
}
