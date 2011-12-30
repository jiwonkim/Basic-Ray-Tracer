#ifndef IMAGE_PLANE_H
#define IMAGE_PLANE_H

#include <cstdlib>
#include "st.h"
#include "Camera.h"

class ImagePlane
{
  public:
    ImagePlane();
    void setImagePlane(int w, int h, std::string _filename, Camera camera);
    bool getPointOnImagePlane(float x, float y, STVector3 &pt);
    void setPixel(int x, int y, STColor3f color);
    STColor3f getPixel(int x, int y);
    void writeToFile();
    void setFisheye();
    void setCylinder(bool x);

  private:
    bool isImagePlaneSet;
    STImage *window;
    std::string filename;

    //camera definition
    Camera camera;
    
    //viewplane
    float fovy;
    STVector3 LL,UL,LR,UR;
    
    //for non-flat lenses
    bool fisheye, cylinderx, cylindery;
    float radius;
};

#endif
