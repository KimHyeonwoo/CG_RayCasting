#ifndef SPHERE_H
#define SPHERE_H
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "material.h"
#include <cmath>

class Sphere : public Shape {
	public:
		Point origin;
		float radius;
    Material mat;
    Sphere (Point o, float r, Material mat);
    Point raycastHit (Ray ray, Point noHit);
		Point getNormal (Point p);
};

#endif
