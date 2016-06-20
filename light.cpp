#include "light.h"

Light :: Light (Point position, Point color) {
  this->position = position;
  this->color = color;
  this->size = 0;
}

Light :: Light (Point position, Point color, float size) {
  this->position = position;
  this->color = color;
  this->size = size;
}
