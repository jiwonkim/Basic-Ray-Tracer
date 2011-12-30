#include "ImagePlane.h"

ImagePlane::ImagePlane() {
  isImagePlaneSet = false; 
  fisheye = false;
  cylinderx = false;
  cylindery = false;
}

void ImagePlane::setImagePlane(int w, int h, std::string _filename, Camera camera) {
  window = new STImage(w,h, STColor4ub(1,1,1));
  filename = _filename;
  
  float fovy_in_radians = DegreesToRadians(camera.getFieldOfViewY());
  fovy = tan(fovy_in_radians/2.);
  STVector3 VY = fovy*camera.getV();
  STVector3 VX = tan(camera.getAspect()*fovy_in_radians/2.)*camera.getU();
  
  STVector3 C = camera.getEye() + camera.getW();
  STVector3 W = camera.getW();
  LL = C + VX - VY;
  UL = C + VX + VY;
  LR = C - VX - VY;
  UR = C - VX + VY;
  
  this->camera = camera;
  radius = (C - VY - camera.getEye()).Length();
  isImagePlaneSet = true;
}

void ImagePlane::setFisheye() {
  fisheye = true;
  cylinderx = false;
  cylindery = false;
}

void ImagePlane::setCylinder(bool x) {
  if(x) {
    cylinderx = true;
    cylindery = false;
  } else {
    cylinderx = false;
    cylindery = true;
  }
  fisheye = false;
}

bool ImagePlane::getPointOnImagePlane(float x, float y, STVector3 &pt) {
  if(!isImagePlaneSet) return false;
  float vpCoordsPerPx = 2*fovy / window->GetHeight();
  float u = -(x-window->GetWidth()/2)*vpCoordsPerPx;
  float v = (y-window->GetHeight()/2)*vpCoordsPerPx;
  
  // If not fisheye, return coordinate on flat plane
  if(!fisheye && !cylinderx && !cylindery) {
    pt = camera.getEye()+u*camera.getU()+v*camera.getV()+camera.getW();
    return true;
  }

  // Calculate the w offset from flat plane
  float offset2;
  if(fisheye) offset2 = radius*radius - u*u - v*v;
  else if(cylinderx) offset2 = radius*radius - u*u;
  else if(cylindery) offset2 = radius*radius - v*v;
  else return false;

  if(offset2 < 0){
    return false;
  }

  // if so, return projected point on lens
  float offset = sqrt(offset2);

  pt = camera.getEye()+u*camera.getU()+v*camera.getV()+(1*offset)*camera.getW();
  return true;
}

void ImagePlane::setPixel(int x, int y, STColor3f color) {
  if(!isImagePlaneSet) return;
  window->SetPixel(x,y,STColor4ub(color));
}

STColor3f ImagePlane::getPixel(int x, int y) {
  if(!isImagePlaneSet) return STColor3f(0,0,0);
  return STColor3f(window->GetPixel(x,y));
}

void ImagePlane::writeToFile() {
  printf("Saving to file\n");
  window->Save(filename);
}
