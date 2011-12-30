#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include "Ray.h"

class Shape
{
  public:
    Shape();
    virtual float intersects(Ray ray);
    virtual STVector3 getNormalAt(STVector3 point, STVector3 direction);
    virtual std::string toString();
  private:
    
};

#endif
