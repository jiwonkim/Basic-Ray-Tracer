#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "st.h"
#include "Light.h"

class PointLight : public Light
{
  public:
    PointLight(const STPoint3 &loc, const STColor3f &col);
    STVector3 getDirectionTo(STVector3 &point);
    STVector3 getLocation(STVector3 &point);
  private:
    STVector3 location;
};

#endif
