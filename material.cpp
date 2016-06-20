#include "material.h"

Material :: Material () {

}

Material :: Material (Point a, Point d, Point s, float alpha, float beta, float ka, float kd, float ks, float g, float n) {
  this->ambient = a;
  this->diffuse = d;
  this->specular = s;
  this->alpha = alpha;
  this->beta = beta;
  this->ambientConst = ka;
  this->diffuseConst = kd;
  this->specularConst = ks;
  this->gloss = g;
  this->n = n;
}
