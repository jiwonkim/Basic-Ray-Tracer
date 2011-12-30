#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "st.h"
#include "SceneObject.h"
#include <cstdlib>

class Intersection
{
  public:
    Intersection(float _t, const STVector3 &_point, const STVector3 &_normal, const STVector3 &_direction, SceneObject *_object);
    STVector3 getNormal();
    STVector3 getDirection(); 
    STVector3 getIntersectionPoint();
    float getTValue();    
    SceneObject *getObject();

  
  private:
    float t;
    STVector3 point;
    STVector3 normal;
    STVector3 direction;
    SceneObject *object;
};

#endif
