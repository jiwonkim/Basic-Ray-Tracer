#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "st.h"
#include "Light.h"

class DirectionalLight : public Light
{
  public:
    DirectionalLight(const STVector3 &dir, const STColor3f &col);
    STVector3 getDirectionTo(STVector3 &point);
    STVector3 getLocation(STVector3 &point);
  private:
    STVector3 direction;
};

#endif
