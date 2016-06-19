#include "ray.h"

Ray :: Ray (Point origin, Point target) {
  this->origin = origin;
  this->dir = (target - origin).normalize();
}
