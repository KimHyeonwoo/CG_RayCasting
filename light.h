#ifndef LIGHT_H
#define LIGHT_H
#include "point.h"

class Light {
  public:
    Point position;
    Point color;
    float size;
    Light (Point position, Point color);
    Light (Point position, Point color, float size);
};

#endif
