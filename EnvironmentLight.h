#ifndef ENVIRONMENT_LIGHT_H
#define ENVIRONMENT_LIGHT_H

#include "st.h"
#include "Light.h"
#include <iostream>
#include <cstdlib>

class EnvironmentLight : public Light
{
  public:
    EnvironmentLight(std::string filename,STColor3f &intensity);
    STColor3f getIntensity(STVector3 &normal);
    STVector3 getDirectionTo(STVector3 &point, STVector3 &normal);
    STVector3 getLocation(STVector3 &point,STVector3 &normal);
  private:
    STImage *lightprobe;
    STColor3f intensity;
};

#endif
