#ifndef CIRCLE_LIGHT_H
#define CIRCLE_LIGHT_H

#include <cstdlib>
#include "st.h"
#include "LineLight.h"

class CircleLight
{
  public:
    CircleLight(const STPoint3 &center, const STVector3 &radiusVector_x, const STVector3 &radiusVector_y, const STColor3f &intensity, int numLines);
    void getPointLights(std::vector<PointLight *> &_lights);
  private:
    struct line {
      bool skipped;
      STVector3 start,end;
      int numSamples;
    };
    std::vector<PointLight *> lights;
    line *lines;
};

#endif
