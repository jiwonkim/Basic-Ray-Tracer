#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "st.h"
#include "Material.h"
#include "AmbientLight.h"
#include "EnvironmentLight.h"
#include "Light.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Ray.h"

class SceneObject
{
  public:
    SceneObject(Shape *_shape, Material _material, STTransform4 transform);
    ~SceneObject();
    STVector3 getNormalAt(STVector3 point, STVector3 direction);
    Ray getUntransformedRay(Ray &ray);
    float intersects(Ray &ray);
    STColor3f getColor(std::vector<AmbientLight *> &ambient_lights, std::vector<Light *> &lights, EnvironmentLight *environment_light, STVector3 &normal, STVector3 &view, STVector3 &point);
    STColor3f getMirrorReflectance();
    STTransform4 getTransform();
    STTransform4 getInverseTransform();
    std::string toString();

  private:
    Shape *shape;
    Material material;
    STTransform4 transform;
    STTransform4 inverse_transform;
};

#endif
