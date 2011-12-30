#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Intersection.h"
#include "ImagePlane.h"
#include "CircleLight.h"
#include "LineLight.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "EnvironmentLight.h"
#include "Shape.h"
#include "Ray.h"
#include "Material.h"
#include "SceneObject.h"

#include "st.h"
#include <cstdlib>

class Scene
{
public:
	Scene(std::string sceneFilename);
  void Render();

private:

	/** Parsing helpers **/
	void Parse(std::string sceneFilename);
	void BeganParsing();
	void FinishedParsing();
  void ParsedCamera(const STPoint3& eye, const STVector3& up, const STPoint3& lookAt, float fovy, float aspect);
	void ParsedOutput(int imgWidth, int imgHeight, const std::string& outputFilename);
	void ParsedBounceDepth(int depth);
	void ParsedShadowBias(float bias);
	void ParsedPushMatrix();
	void ParsedPopMatrix();
	void ParsedRotate(float rx, float ry, float rz);
	void ParsedScale(float sx, float sy, float sz);
	void ParsedTranslate(float tx, float ty, float tz);
	void ParsedSphere(const STPoint3& center, float radius);
	void ParsedTriangle(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3);
	void ParsedLineLight(const STPoint3& start, const STPoint3& end, const STColor3f& intensity, const int numSamples);
	void ParsedCircleLight(const STPoint3& center, const STVector3& radiusVector_x, const STVector3& radiusVector_y, const STColor3f &intensity, int numLines);
  void ParsedEnvironmentLight(const std::string filename, STColor3f &intensity);
  void ParsedAmbientLight(const STColor3f& col);
	void ParsedPointLight(const STPoint3& loc, const STColor3f& col);
	void ParsedDirectionalLight(const STVector3& dir, const STColor3f& col);
	void ParsedMaterial(const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, const STColor3f& mirr, float shine);

  /** Ray tracing helpers **/
  Intersection *FindIntersection(Ray ray);
  STColor3f RecursivelyRayTrace(Intersection *hit, STColor3f &color, int bounce);
  STColor3f FindColorAt(Intersection *hit);
  void FindReachableLights(Intersection *hit, std::vector<Light *> &reachable);

	/** CS 148 TODO: Add instance vars to store camera, lights, objects, etc. **/
  ImagePlane imageplane;
  Camera camera;
  Material material;
  int width, height;
  float bounce_depth, shadow_bias;

  std::vector<AmbientLight *> ambient_lights;
  std::vector<Light *> lights;  
  EnvironmentLight *environment_light;
  std::vector<SceneObject> objects;
  std::vector<STTransform4> matrix_stack;
  
};


#endif SCENE_H

