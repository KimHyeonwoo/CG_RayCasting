#ifndef RAY_H
#define RAY_H
#include "point.h"

class Ray {
	public:
		Point origin;
		Point dir;
    Ray (Point origin, Point target);
};

#endif
