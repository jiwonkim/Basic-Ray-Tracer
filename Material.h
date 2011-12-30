#ifndef MATERIAL_H
#define MATERIAL_H

#include "st.h"

class Material
{
  public:
    Material();
    void setMaterial(const STColor3f &ambient, const STColor3f &diffuse, const STColor3f &specular, const STColor3f & mirror, const float shininess);
    
    bool materialSet;
    STColor3f ambient, diffuse, specular, mirror;
    float shininess;
};

#endif
