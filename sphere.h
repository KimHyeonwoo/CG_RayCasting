#ifndef SPHERE_H
#define SPHERE_H
#include "point.h"
#include "ray.h"
#include "shape.h"
#include <cmath>

class Sphere : public Shape {
	public:
		Point origin;
		float radius;
		Sphere (Point o, float r);
    Point raycastHit (Ray ray, Point noHit);
		Point getNormal (Point p);
};

#endif
