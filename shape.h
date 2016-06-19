#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include "ray.h"

class Shape {
	public:
		virtual Point raycastHit (Ray ray);
		virtual Point getNormal (Point p);
};

#endif
