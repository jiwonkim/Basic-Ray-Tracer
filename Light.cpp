#include "Light.h"

Light::Light() {
  color = STColor3f(0,0,0);
}

Light::Light(const STColor3f &_color) {
  color = _color; 
}

STColor3f Light::getIntensity() {
  return color;
}

STVector3 Light::getDirectionTo(STVector3 &point) {
  return STVector3(0,0,0);
}

STVector3 Light::getLocation(STVector3 &point) {
  return STVector3(0,0,0);
}
