#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"

#include <fstream>
#include <sstream>
#include <stdlib.h> 
#include <time.h>

#define NUM_JITTER 3

Scene::Scene(std::string sceneFilename)
{
  srand(time(NULL));
	Parse(sceneFilename);
}

/*
 * function Render
 * ---------------
 * Renders the image to an image file described in the file parsed
 */
void Scene::Render() { 
  // Iterates through each pixel in image, shoots ray through it
  for(int y = height-1; y >= 0; y--) {
    for(int x = 0; x < width; x++) {
      
      STColor3f sum = STColor3f(0,0,0);
      float jitter = 1./(NUM_JITTER+1);
      for(int i = 1; i <= NUM_JITTER; i++) {
        for(int j = 1; j <= NUM_JITTER; j++) {
          float jx = i*jitter;
          float jy = j*jitter;
          // Create ray through center of pixel (x,y)
          STVector3 point;
          if(!imageplane.getPointOnImagePlane(x+jx,y+jy,point)) continue;
          Ray ray = Ray(camera, point);
          
          // Find the closest intersection of ray and object
          Intersection *hit = FindIntersection(ray);

          // If ray hit any object, find the color at the pixel (x,y) by
          // getting the color of the intersection and recursing through
          // all refelctive rays.
          if(hit!=NULL && hit->getTValue()>1) {
            STColor3f color = FindColorAt(hit);
            sum += RecursivelyRayTrace(hit,color,1);
          }
        }
      }
      sum /= (float)(NUM_JITTER*NUM_JITTER);
      imageplane.setPixel(x,y,sum);

      //debug: show progress
      if(x%5==0 && y%10==0){
        //double val = sqrt(sum.r*sum.r+sum.g*sum.g+sum.b*sum.b);
        double val =0.3*sum.r + 0.59*sum.g + 0.11*sum.b;
        if(val<0.1){
          printf(" ");
        } else if(val<0.4){
          printf(".");
        } else if (val < 0.6){
          printf("*");
        } else if (val < 0.8){
          printf("#");
        } else {
          printf("@");
        }
        fflush(stdout);
      }
    }
    if(y%10==0){
      printf("\n");
    }
  }
  printf("Iterated over all pixels\n");
  
  int num_ambient_lights = ambient_lights.size();
  for(int i=0; i<num_ambient_lights; i++) {
    delete ambient_lights[i];
  }

  int num_lights = lights.size();
  for(int i=0; i<num_lights; i++) {
    delete lights[i];
  }

  delete environment_light;
  printf("Freed memory\n");
  imageplane.writeToFile(); // Generate resulting image file
}

Intersection *Scene::FindIntersection(Ray ray) {
  // Total number of objects in scene
  int num_objects = objects.size();
  
  // True when the ray hasn't hit any object yet
  bool flag = true;

  // t value where the ray intersects the closest object to source point
  float min_t;

  // index of the closest object hit by ray
  int object_index;

  // Untransformed version of the ray that hit above object - differs from object to object
  Ray untransformed_ray;

  // Iterate through each object, try intersecting it with ray
  for(int i=0; i<num_objects; i++) {
    Ray r = objects[i].getUntransformedRay(ray);
    float t = objects[i].intersects(r);

    if(t>0 && (flag || t<min_t)) {
      min_t = t;
      flag = false;
      object_index = i;
      untransformed_ray = r;
    }
  }
  
  // If no object has hit, return NULL
  if(flag) return NULL;

  // Get transformation matrix and its inverse in order to get true point and normal
  STTransform4 transform = objects[object_index].getTransform();
  STTransform4 inverse_transform = objects[object_index].getInverseTransform();

  // Given the info we have of the untransformed point, create the actual point in scene
  STVector3 untransformed_point = untransformed_ray.getPointAt(min_t);
  STVector3 point = STVector3(transform*STPoint3(untransformed_point));

  // Given the normal generated with the untransformed ray and object, get the actual normal
  STVector3 untransformed_normal = objects[object_index].getNormalAt(untransformed_point,untransformed_ray.getDirection());
  STVector3 normal = STVector3(inverse_transform.Transpose()*STPoint3(untransformed_normal));
  
  // Return an Intersection object created with actual point, normal, ray direction, and object
  return new Intersection(min_t,point,normal,ray.getDirection(),&objects[object_index]); 
}


/*
 * Find if the given intersection is shadowed by shooting rays to each light source
 */
void Scene::FindReachableLights(Intersection *hit,std::vector<Light *> &reachable) {
  STVector3 point = hit->getIntersectionPoint();
  STVector3 normal = hit->getObject()->getNormalAt(point,camera.getEye()-point);

  // Loop over each light and shoot a ray to the light
  int num_lights = lights.size();
  for(int i=0; i<num_lights; i++) {
    STVector3 lightSource = lights[i]->getLocation(point);
    STVector3 direction = lightSource - point;
    
    Ray ray = Ray(point+shadow_bias*direction,lightSource);
    Intersection *obstacle = FindIntersection(ray);
    
    if(obstacle==NULL) reachable.push_back(lights[i]);
    if(obstacle!=NULL) {
      float t = obstacle->getTValue();
      if(t<=0 || t>=1) reachable.push_back(lights[i]);
    }
  }
}


STColor3f Scene::RecursivelyRayTrace(Intersection *hit, STColor3f &color, int bounce) {
  if(bounce>bounce_depth) return color;
  
  // mirror reflectance of material - if less than epsilon, return curr color
  STColor3f mirror = (hit->getObject())->getMirrorReflectance();
  if(mirror.r*mirror.g*mirror.b < 0.0001) return color;

  // source point of ray
  STVector3 source = hit->getIntersectionPoint();

  // calculates reflection vector using normal and direction of previous ray
  STVector3 normal = hit->getNormal();
  normal /= normal.Length();
  STVector3 prev_dir = hit->getDirection();
  prev_dir /= prev_dir.Length();
  STVector3 next_dir = prev_dir-2*STVector3::Dot(prev_dir,normal)*normal;
  next_dir /= next_dir.Length();
  
  // generates new ray from new source
  Ray ray = Ray(source+next_dir*shadow_bias, source+next_dir);
  Intersection *next_hit = FindIntersection(ray);
  
  // if ray did not hit anything, return current color - nothing to reflect
  if(next_hit==NULL) return color;
  
  // evaluate mirror reflectance and update
  color += mirror*FindColorAt(next_hit);
  return RecursivelyRayTrace(next_hit,color,bounce+1);
}



STColor3f Scene::FindColorAt(Intersection *hit) {
  STVector3 normal = hit->getNormal();
  STVector3 point = hit->getIntersectionPoint();
  STVector3 view = camera.getEye() - point;
  
  SceneObject *object = hit->getObject();
  std::vector<Light *> reachable_lights;
  FindReachableLights(hit,reachable_lights);
  
  // Pass in all ambient lights and environment lights for computing color because
  // they are always reachable, but pass in only reachable lights for point/directional
  // lights because they can generate shadows
  return object->getColor(ambient_lights,reachable_lights,environment_light,normal,view,point);
}
void Scene::Parse(std::string sceneFilename)
{
	BeganParsing();

	std::ifstream sceneFile(sceneFilename.c_str());
	char line[1024];
	while (!sceneFile.eof())
	{
		sceneFile.getline(line, 1023);
		std::stringstream ss;
		ss.str(line);
		std::string command;
		ss >> command;

    if (command == "Fisheye") {
      imageplane.setFisheye();
    }
    else if (command == "Cylinderx") {
      imageplane.setCylinder(true);
    }
    else if (command == "Cylindery") {
      imageplane.setCylinder(false);
    }
		else if (command == "Camera")
		{
			float ex, ey, ez, ux, uy, uz, lx, ly, lz, f, a;
			ss >> ex >> ey >> ez >> ux >> uy >> uz >> lx >> ly >> lz >> f >> a;
			STPoint3 eye(ex, ey, ez);
			STVector3 up(ux, uy, uz);
			STPoint3 lookAt(lx, ly, lz);
			ParsedCamera(eye, up, lookAt, f, a);
		}
		else
		if (command == "Output")
		{
			int w, h;
			std::string fname;
			ss >> w >> h >> fname;
			ParsedOutput(w, h, fname);
		}
		else
		if (command == "BounceDepth")
		{
			int depth;
			ss >> depth;
			ParsedBounceDepth(depth);
		}
		else if (command == "ShadowBias")
		{
			float bias;
			ss >> bias;
			ParsedShadowBias(bias);
		}
		else
		if (command == "PushMatrix")
		{
			ParsedPushMatrix();
		}
		else
		if (command == "PopMatrix")
		{
			ParsedPopMatrix();
		}
		else
		if (command == "Rotate")
		{
			float rx, ry, rz;
			ss >> rx >> ry >> rz;
			ParsedRotate(rx, ry, rz);
		}
		else
		if (command == "Scale")
		{
			float sx, sy, sz;
			ss >> sx >> sy >> sz;
			ParsedScale(sx, sy, sz);
		}
		else
		if (command == "Translate")
		{
			float tx, ty, tz;
			ss >> tx >> ty >> tz;
			ParsedTranslate(tx, ty, tz);
		}
		else
		if (command == "Sphere")
		{
			float cx, cy, cz, r;
			ss >> cx >> cy >> cz >> r;
			STPoint3 center(cx, cy, cz);
			ParsedSphere(center, r);
		}
		else
		if (command == "Triangle")
		{
			float x1, y1, z1, x2, y2, z2, x3, y3, z3;
			ss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
			STPoint3 v[3];
			v[0] = STPoint3(x1, y1, z1);
			v[1] = STPoint3(x2, y2, z2);
			v[2] = STPoint3(x3, y3, z3);
			ParsedTriangle(v[0], v[1], v[2]);
		}
		else
    if (command == "EnvironmentLight")
		{
      std::string filename;
      float r,g,b;
      ss >> filename >> r >> g >> b;
      STColor3f intensity(r,g,b);
      ParsedEnvironmentLight(filename, intensity);
    }
    else
    if (command == "AmbientLight")
		{
			float r, g, b;
			ss >> r >> g >> b;
			STColor3f col(r, g, b);
			ParsedAmbientLight(col);
		}
		else
		if (command == "PointLight")
		{
			float px, py, pz, r, g, b;
			ss >> px >> py >> pz >> r >> g >> b;
			STPoint3 pos(px, py, pz);
			STColor3f col(r, g, b);
			ParsedPointLight(pos, col);
		}
		else
		if (command == "DirectionalLight")
		{
			float dx, dy, dz, r, g, b;
			ss >> dx >> dy >> dz >> r >> g >> b;
			STVector3 dir(dx, dy, dz);
			STColor3f col(r, g, b);
			ParsedDirectionalLight(dir, col);
		}
		else
    if (command == "LineLight") {
      float sx, sy, sz, ex, ey, ez, r, g, b;
      int n;
      ss >> sx >> sy >> sz >> ex >> ey >> ez >> r >> g >> b >> n;
      STPoint3 start(sx,sy,sz);
      STPoint3 end(ex,ey,ez);
      STColor3f intensity(r,g,b);
      ParsedLineLight(start,end,intensity,n);
    }
    else
    if (command == "CircleLight") {
      float cx,cy,cz,rxx,rxy,rxz,ryx,ryy,ryz,r,g,b;
      int n;
      ss >> cx >> cy >> cz >> rxx >> rxy >> rxz >> ryx >> ryy >> ryz >> r >> g >> b >> n;
      STPoint3 center(cx,cy,cz);
      STVector3 radiusVector_x(rxx,rxy,rxz);
      STVector3 radiusVector_y(ryx,ryy,ryz);
      STColor3f intensity(r,g,b);
      ParsedCircleLight(center,radiusVector_x,radiusVector_y,intensity,n);
    }
		else
    if (command == "Material")
		{
			float ra, ga, ba, rd, gd, bd, rs, gs, bs, rr, gr, br, shine;
			ss >> ra >> ga >> ba >> rd >> gd >> bd >> rs >> gs >> bs >> rr >> gr >> br >> shine;
			STColor3f amb(ra, ga, ba);
			STColor3f diff(rd, gd, bd);
			STColor3f spec(rs, gs, bs);
			STColor3f mirr(rr, gr, br);
			ParsedMaterial(amb, diff, spec, mirr, shine);
		}
	}
	sceneFile.close();

	FinishedParsing();
}

void Scene::BeganParsing()
{
  environment_light = NULL;
  matrix_stack.push_back(STTransform4::Identity());
  printf("Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }
}

void Scene::FinishedParsing()
{
  matrix_stack.clear();
  printf("\nNumber of non-ambient lights in scene: %d\n",(int)lights.size());
  printf("Number of objects in scene: %d\n",(int)objects.size());
  printf("Finished parsing file\n");
  
}

void Scene::ParsedCamera(const STPoint3& eye, const STVector3& up, const STPoint3& lookAt, float fovy, float aspect)
{
  camera.setCamera(eye,up,lookAt,fovy,aspect);
}

void Scene::ParsedOutput(int imgWidth, int imgHeight, const std::string& outputFilename)
{
  width = imgWidth, height = imgHeight;
  imageplane.setImagePlane(imgWidth,imgHeight,outputFilename,camera);
}

void Scene::ParsedBounceDepth(int depth)
{
  bounce_depth = depth;
}

void Scene::ParsedShadowBias(float bias)
{
  shadow_bias = bias;
}

void Scene::ParsedPushMatrix()
{
  STTransform4 matrix = STTransform4(matrix_stack.back());
  matrix_stack.push_back(matrix);

  printf("Pushed top matrix. Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }
}

void Scene::ParsedPopMatrix()
{
  matrix_stack.pop_back();
  printf("\nPopped matrix. Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }
}

void Scene::ParsedRotate(float rx, float ry, float rz)
{
  STTransform4 rotation_matrix = STTransform4::Rotation(DegreesToRadians(rx),DegreesToRadians(ry),DegreesToRadians(rz));
  int i = matrix_stack.size() - 1;
  matrix_stack[i] = matrix_stack[i]*rotation_matrix;
  printf("Rotated top matrix. Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }

}

void Scene::ParsedScale(float sx, float sy, float sz)
{
  STTransform4 scaling_matrix = STTransform4::Scaling(sx,sy,sz);
  int i = matrix_stack.size() - 1;
  matrix_stack[i] = matrix_stack[i]*scaling_matrix;
  printf("Scaled top matrix. Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }


}

void Scene::ParsedTranslate(float tx, float ty, float tz)
{
  STTransform4 translation_matrix = STTransform4::Translation(tx,ty,tz);
  int i = matrix_stack.size() - 1;
  matrix_stack[i] = matrix_stack[i]*translation_matrix;
  printf("Translated top matrix. Transformation stack looks like this: \n");
  for(int i=0; i<matrix_stack.size(); i++) {
    for(int r=0; r<4; r++) {
      float *row = matrix_stack[i][r];
      printf(" %f %f %f %f\n",row[0],row[1],row[2],row[3]);
    }
    printf("\n");
  }


}

void Scene::ParsedSphere(const STPoint3& center, float radius)
{
  objects.push_back(SceneObject(new Sphere(center,radius),material,matrix_stack.back()));
}

void Scene::ParsedTriangle(const STPoint3& v1, const STPoint3& v2, const STPoint3& v3)
{
  objects.push_back(SceneObject(new Triangle(v1,v2,v3),material,matrix_stack.back()));
}

void Scene::ParsedLineLight(const STPoint3 &start, const STPoint3 &end, const STColor3f &intensity, const int numSamples) 
{
  LineLight *light = new LineLight(start,end,intensity,numSamples);
  
  std::vector<PointLight *> point_lights;
  light->getPointLights(point_lights);
  for(int i=0; i<numSamples; i++) {
    lights.push_back(point_lights[i]);
  }
}

void Scene::ParsedCircleLight(const STPoint3& center, const STVector3& radiusVector_x, const STVector3& radiusVector_y, const STColor3f &intensity, int numLines) {
  CircleLight *light_source = new CircleLight(center,radiusVector_x,radiusVector_y,intensity,numLines);
  
  std::vector<PointLight *> point_lights;
  light_source->getPointLights(point_lights);
  int num_lights = point_lights.size();
  for(int i=0; i<num_lights; i++) {
    lights.push_back(point_lights[i]);
  }
}

void Scene::ParsedEnvironmentLight(const std::string filename, STColor3f &intensity) {
  environment_light = new EnvironmentLight(filename, intensity);
}

void Scene::ParsedAmbientLight(const STColor3f& col)
{
  ambient_lights.push_back(new AmbientLight(col));
}

void Scene::ParsedPointLight(const STPoint3& loc, const STColor3f& col)
{
  lights.push_back(new PointLight(matrix_stack.back()*loc,col));
}

void Scene::ParsedDirectionalLight(const STVector3& dir, const STColor3f& col)
{
  lights.push_back(new DirectionalLight(matrix_stack.back()*dir,col));
}

void Scene::ParsedMaterial(const STColor3f& amb, const STColor3f& diff, const STColor3f& spec, const STColor3f& mirr, float shine)
{
  material.setMaterial(amb,diff,spec,mirr,shine);
}

