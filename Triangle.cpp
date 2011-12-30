#include "Triangle.h"
#include <math.h>

Triangle::Triangle(const STPoint3 &_v0, const STPoint3 &_v1, const STPoint3 &_v2) {
  v0 = _v0, v1 = _v1, v2 = _v2;
}

float Triangle::intersects(Ray ray) {
  STVector3 E = ray.getEye();
  STVector3 D = ray.getDirection();
  float a,b,c,d,e,f,g,h,i,j,k,l;
  a = v0.x - v1.x;
  b = v0.y - v1.y;
  c = v0.z - v1.z;
  d = v0.x - v2.x;
  e = v0.y - v2.y;
  f = v0.z - v2.z;
  g = D.x;
  h = D.y;
  i = D.z;
  j = v0.x - E.x;
  k = v0.y - E.y;
  l = v0.z - E.z;

  float ei_hf = e*i - h*f;
  float gf_di = g*f - d*i;
  float bl_kc = b*l - k*c;
  float ak_jb = a*k - j*b;
  float jc_al = j*c - a*l;
  float dh_eg = d*h - e*g;
  float M = a*ei_hf + b*gf_di + c*dh_eg;

  float beta, gamma;
  beta = (j*ei_hf + k*gf_di + l*dh_eg)/M;
  gamma = (i*ak_jb + h*jc_al + g*bl_kc)/M;

  // Triangle doesn't intersect with ray - return negative
  if(!(beta>=0 && gamma>=0 && beta+gamma<=1)) return -1;
  
  // Ray intersects with triangle, return t value
  float t = -(f*ak_jb + e*jc_al + d*bl_kc)/M;
  return t;
}

    
STVector3 Triangle::getNormalAt(STVector3 point, STVector3 direction) {
  STVector3 normal = STVector3::Cross(v1-v0,v2-v0);
  float angle0 = acos(STVector3::Dot(-direction,normal)/(direction.Length()*normal.Length()));
  float angle1 = acos(STVector3::Dot(-direction,-normal)/(direction.Length()*normal.Length()));
  if(angle0<angle1) return normal;
  return -normal;
}

std::string Triangle::toString() {
  return "Triangle Object";
}
