#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include "ray.h"

class Shape {
	public:
		Point raycastHit (Ray ray, Point noHit);
		Point getNormal (Point p);
};

#endif
