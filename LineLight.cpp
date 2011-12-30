#include "LineLight.h"
#include <stdio.h>

LineLight::LineLight(const STPoint3 &start, const STPoint3 &end, const STColor3f &intensity, int numSamples) {
  STVector3 step = (end-start)/((float)numSamples-1.);
  for(int i=0; i<numSamples; i++) {
    lights.push_back(new PointLight(start+step*i, intensity/(float)numSamples));
  }
}

void LineLight::getPointLights(std::vector<PointLight *> &_lights) {
  _lights = lights;
}

