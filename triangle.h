#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "point.h"
#include "ray.h"
#include "shape.h"
#include <cmath>

class Triangle : public Shape {
  public:
    Point p1;
    Point p2;
    Point p3;
    Triangle (Point p1, Point p2, Point p3);
    Point raycastHit (Ray ray, Point noHit);
    Point getNormal (Point p);
};

#endif
