#include "SceneObject.h"
#include <cstdlib>
#include <math.h>
#define NUM_NORMALS 20

SceneObject::SceneObject(Shape *_shape, Material _material, STTransform4 _transform) {
  shape = _shape;
  material = _material;
  transform = _transform;
  inverse_transform = _transform.Inverse();
  
  printf("Created SceneObject with transformation matrix: \n");
  for(int r=0; r<4; r++) {
    float *row = transform[r];
    printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
  }
  printf("\n");
}

SceneObject::~SceneObject() {
}


STVector3 SceneObject::getNormalAt(STVector3 point, STVector3 direction) {
  return shape->getNormalAt(point, direction);
}

Ray SceneObject::getUntransformedRay(Ray &ray) {
  return ray.getTransformedRay(inverse_transform);
}

STTransform4 SceneObject::getInverseTransform() {
  return inverse_transform;
}

STTransform4 SceneObject::getTransform() {
  return transform;
}

float SceneObject::intersects(Ray &ray) {
  return shape->intersects(ray);
}

STColor3f SceneObject::getColor(std::vector<AmbientLight *> &ambient_lights, std::vector<Light *> &lights, EnvironmentLight *environment_light, STVector3 &normal, STVector3 &view, STVector3 &point) {
  normal /= normal.Length(); // make sure vectors are normalized
  view /= view.Length();

  STColor3f color = STColor3f(0,0,0); // color to return at end of function
  
  // Sum over all intensities of ambient lights
  int num_ambient_lights = ambient_lights.size();
  for(int i=0; i<num_ambient_lights; i++) {
    color += material.ambient*ambient_lights[i]->getIntensity();
  }

  // Sum over all intensities of other lights, taking into account the direction vectors
  STColor3f sum_light = STColor3f(0,0,0);
  int num_lights = lights.size();
  for(int i=0; i<num_lights; i++) {
    STColor3f I = lights[i]->getIntensity();
    
    STVector3 L = lights[i]->getDirectionTo(point); 
    L /= L.Length();
    float L_dot_N = STVector3::Dot(L,normal);
    
    STVector3 R = -L-2*STVector3::Dot(-L,normal)*normal; 
    R /= R.Length();
    float R_dot_V = STVector3::Dot(R,view);

    color += material.diffuse*I*std::max((float)0,L_dot_N); 
    color += material.specular*I*pow(std::max((float)0,R_dot_V),material.shininess);
  }

  if(environment_light==NULL) return color; 
  // Calculations for environment-mapped lighting
  color += material.mirror*environment_light->getIntensity(normal);
    
  // Generate a set number of faux-normals
  STColor3f sum_environment = STColor3f(0,0,0);
  int num_normals = 0;
  while(num_normals < NUM_NORMALS) {
    // Get random values for x, y, and z where
    // x has range [-1,1]
    // y has range [-1,1]
    // z has range [ 0,1]

    float x = rand()*2./(float)RAND_MAX - 1.;
    float y = rand()*2./(float)RAND_MAX - 1.;
    float z = rand()*2./(float)RAND_MAX;

    // repeat until resulting vector (x,y,z) has length < 1
    if(x*x + y*y + z*z > 1.) continue;
    
    
    // Calculate vectors to multiply x, y, and z by
    STVector3 u;
    if(normal.z!=0.) u = STVector3(1,1,-(normal.x+normal.y)/normal.z);
    else if(normal.y!=0.) u = STVector3(1,-(normal.x+normal.z)/normal.y, 1);
    else if(normal.x!=0.) u = STVector3(-(normal.y+normal.z)/normal.x,1,1);
    else return color;
    
    STVector3 v = STVector3::Cross(normal,u);
    
    // Generate new normal and its intensity, based on its angle with N
    STVector3 new_normal = x*u + y*v + z*normal;
    new_normal /= new_normal.Length();

    // Calculate diffuse and specular lighting as usual
    STColor3f I = environment_light->getIntensity(new_normal);
    
    STVector3 L = environment_light->getDirectionTo(point, new_normal); 
    L /= L.Length();
    float L_dot_N = STVector3::Dot(L,new_normal);
    
    STVector3 R = -L-2*STVector3::Dot(-L,new_normal)*new_normal; 
    R /= R.Length();
    float R_dot_V = STVector3::Dot(R,view);

    sum_environment += material.diffuse*I*std::max((float)0,L_dot_N); 
    sum_environment += material.specular*I*pow(std::max((float)0,R_dot_V),material.shininess);
    ++num_normals;
  }

  color += sum_environment/(float)NUM_NORMALS;

  return color;
}

STColor3f SceneObject::getMirrorReflectance() {
  return material.mirror;
}

std::string SceneObject::toString() {
  return shape->toString();
}
