#include "CircleLight.h"
#include <cstdlib>
#include <stdio.h>

CircleLight::CircleLight(const STPoint3 &center, const STVector3 &radiusVector_x, const STVector3 &radiusVector_y, const STColor3f &intensity, int numLines) {
  printf("In CircleLight: numLines is %d\n",numLines);
  lines = new line[numLines];
  
  STVector3 C = STVector3(center);
  STVector3 u = radiusVector_x*2.0/((float)numLines-1);
  STVector3 v = radiusVector_y*2.0/((float)numLines-1);
  float r = (radiusVector_x.Length() + radiusVector_y.Length())/(2.0*u.Length());

  // Iterate through each line slicing circle, create LineLight
  int totalSamples = 0;
  for(int l=0; l<numLines; l++) {

    // This is the center point of the line light
    STVector3 center_vector = l*u-radiusVector_x;
    
    // x is in units of the vector u
    float x;
    if(l<numLines/2.0) x = -center_vector.Length()/u.Length();
    else x = center_vector.Length()/u.Length();

    // y is in units of the vector y
    float offset = r*r - x*x;
    if(offset<-0.0001) {
      lines[l].skipped = true;
      continue;
    }
    lines[l].skipped = false;
    float y = sqrt(abs(offset));
    
    // Put computed info to array of line structs
    lines[l].start = x*u - y*v + C;
    lines[l].end = x*u + y*v + C;
    if(l==0 || l==numLines-1) lines[l].numSamples = 1; 
    else lines[l].numSamples = numLines*(y/r); 

    // Update number of total samples
    totalSamples += lines[l].numSamples;
  }

  STColor3f intensity_per_sample = intensity/(float)totalSamples;
  printf("Total samples in CircleLight: %d\n",totalSamples);
  for(int i=0; i<numLines; i++) {
    if(lines[i].skipped) continue;

    STColor3f line_intensity = intensity_per_sample*lines[i].numSamples;
    LineLight *light = new LineLight(STPoint3(lines[i].start),STPoint3(lines[i].end),line_intensity,lines[i].numSamples);
    
    std::vector<PointLight *> lights_for_line;
    light->getPointLights(lights_for_line);
    int num_lights = lights_for_line.size();
    for(int j=0; j<num_lights; j++) {
      lights.push_back(lights_for_line[j]);
    }
  }

}

void CircleLight::getPointLights(std::vector<PointLight *> &_lights) {
  _lights = lights;
}

