#include "triangle.h"

Triangle :: Triangle (Point p1, Point p2, Point p3, Material mat) {
  this->p1 = p1;
  this->p2 = p2;
  this->p3 = p3;
  this->mat = mat;
}

Point Triangle :: raycastHit (Ray ray, Point noHit) {
  Point n = getNormal(p1);
  float d = n * p1;
  if (n * ray.dir < 0.000001 && n * ray.dir > -0.000001) return noHit;
  float s = (d - n * ray.origin) / (n * ray.dir);
  if (s < 0) return noHit;
  Point hit = ray.origin + ray.dir * s;
  Point u = p2 - p1;
  Point v = p3 - p1;
  Point w = hit - p1;
  float s1 = ((u * v) * (w * v) - (v * v) * (w * u)) / ((u * v) * (u * v) - (u * u) * (v * v));
  float s2 = ((u * v) * (w * u) - (u * u) * (w * v)) / ((u * v) * (u * v) - (u * u) * (v * v));
  if (s1 >= 0 && s2 >= 0 && s1 + s2 <= 1) return hit;
  else return noHit;
}

Point Triangle :: getNormal (Point p) {
  return ((p2 - p1).crossProduct(p3 - p1)).normalize();
}
