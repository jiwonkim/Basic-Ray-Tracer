#include "Material.h"

Material::Material() {
}

void Material::setMaterial(const STColor3f &_ambient, const STColor3f &_diffuse, const STColor3f &_specular, const STColor3f &_mirror, const float _shininess) {
  ambient = _ambient;
  diffuse = _diffuse;
  specular = _specular;
  mirror = _mirror;
  shininess = _shininess;
}
