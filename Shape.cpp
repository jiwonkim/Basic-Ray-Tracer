#include "Shape.h"

Shape::Shape() {
}

float Shape::intersects(Ray ray) {
  return -1;
}


STVector3 Shape::getNormalAt(STVector3 point, STVector3 direction) {
  return STVector3(0,0,0);
}

std::string Shape::toString() {
  return "Shape Object";
}
