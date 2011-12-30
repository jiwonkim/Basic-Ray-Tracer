#include "AmbientLight.h"

AmbientLight::AmbientLight(const STColor3f &color) : Light(color)
{
}

STVector3 AmbientLight::getDirectionTo(STVector3 &point) {
  return STVector3(0,0,0);
}

