#include "EnvironmentLight.h"

EnvironmentLight::EnvironmentLight(std::string filename, STColor3f &_intensity) {
  lightprobe = new STImage(filename);
  intensity = _intensity;
  printf("\nEnvironmentLight, width %d, height %d\n",lightprobe->GetWidth(),lightprobe->GetHeight());
}

STColor3f EnvironmentLight::getIntensity(STVector3 &normal) {
  normal /= normal.Length(); // also the coord of point on probe
  
  float tex_x =0.5*normal.x + 0.5;
  float tex_y =0.5*normal.y + 0.5;
  int px_x = tex_x*lightprobe->GetWidth();
  int px_y = tex_y*lightprobe->GetHeight();
  
  px_x = std::min(px_x,lightprobe->GetWidth()-1);
  px_y = std::min(px_y,lightprobe->GetHeight()-1);
  
  return STColor3f(lightprobe->GetPixel(px_x,px_y))*intensity;
}

// Gets direction to light from point
STVector3 EnvironmentLight::getDirectionTo(STVector3 &point, STVector3 &normal) {
  return normal;
}

// Gets location of source light, based on the point from which light is being looked at
STVector3 EnvironmentLight::getLocation(STVector3 &point,STVector3 &normal) {
  return point+normal;
}
