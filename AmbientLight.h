#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "Light.h"

class AmbientLight : public Light
{
  public:
    AmbientLight(const STColor3f &color);
    STVector3 getDirectionTo(STVector3 &point);
  private:
};

#endif
