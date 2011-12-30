#ifndef CAMERA_H
#define CAMERA_H

#include "st.h"

class Camera
{
  public:
    Camera();
    void setCamera(const STPoint3 &_eye, const STVector3 &_up, const STPoint3 &_lookAt, float _fovy, float _aspect);
    float getFieldOfViewY();
    float getAspect();
    STVector3 getW();
    STVector3 getU();
    STVector3 getV();
    STVector3 getEye();
  private:
    bool cameraSet;
    STVector3 eye, lookAt, up;
    float fovy, aspect;

    // orthonormal bases
    STVector3 w,u,v;
};

#endif
