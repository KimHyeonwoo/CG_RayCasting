#ifndef MATERIAL_H
#define MATERIAL_H
#include "point.h"

class Material {
  public:
    Point ambient;
    Point diffuse;
    Point specular;
    float alpha;
    float beta;
    float ambientConst;
    float diffuseConst;
    float specularConst;
    float gloss;
    tmp.print();
    Material (Point a, Point d, Point s, float alpha, float beta, float ka, float kd, float ks, float g);
};  

#endif
