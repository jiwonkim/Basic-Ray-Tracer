#ifndef LIGHT_H
#define LIGHT_H

#include "st.h"

class Light
{
  public:
    Light();
    Light(const STColor3f &_color);
    virtual STColor3f getIntensity();
    virtual STVector3 getDirectionTo(STVector3 &point);
    virtual STVector3 getLocation(STVector3 &point);
  private:
    STColor3f color;
};

#endif
