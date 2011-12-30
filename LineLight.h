#ifndef LINE_LIGHT_H
#define LINE_LIGHT_H

#include <cstdlib>
#include "st.h"
#include "PointLight.h"

class LineLight
{
  public:
    LineLight(const STPoint3 &start, const STPoint3 &end, const STColor3f &intensity, int numSamples);
    void getPointLights(std::vector<PointLight *> &_lights);
  private:
    std::vector<PointLight *> lights;
};

#endif
