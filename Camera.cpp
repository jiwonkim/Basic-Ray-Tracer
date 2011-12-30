#include "Camera.h"

Camera::Camera() {
  cameraSet = false;
}

void Camera::setCamera(const STPoint3 &_eye, const STVector3 &_up, const STPoint3 &_lookAt, float _fovy, float _aspect) {
  // store camera location, the up vector, and the position to look at
  eye = STVector3(_eye);
  up = _up;
  lookAt = STVector3(_lookAt);
  
  // set orthonormal bases
  STVector3 lookAtVector = lookAt-eye;
  w = lookAtVector/lookAtVector.Length();
  
  STVector3 upxw = STVector3::Cross(up,w);
  u = upxw/upxw.Length();

  v = STVector3::Cross(w,u);

  // Store fovy and aspect for later reference
  fovy = _fovy;
  aspect = _aspect;
  cameraSet = true;
}

float Camera::getFieldOfViewY() {
  if(!cameraSet) return 0.0;
  return fovy;
}

float Camera::getAspect() {
  if(!cameraSet) return 0.0;
  return aspect;
}

STVector3 Camera::getEye() {
  if(!cameraSet) return STVector3(0,0,0);
  return eye;
}

STVector3 Camera::getW() {
  if(!cameraSet) return STVector3(0,0,0);
  return w;
}

STVector3 Camera::getU() {
  if(!cameraSet) return STVector3(0,0,0);
  return u;
}

STVector3 Camera::getV() {
  if(!cameraSet) return STVector3(0,0,0);
  return v;
}


