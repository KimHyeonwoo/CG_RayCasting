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
		virtual Point raycastHit (Ray ray);
		virtual Point getNormal (Point p);
};

#endif
