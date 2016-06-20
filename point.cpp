#include "point.h"
#include <cmath>

Point :: Point() {
	x = y = z = 0;
}

Point :: Point (float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point Point :: operator+ (const Point& p) {
	Point ret (x + p.x, y + p.y, z + p.z);
	return ret;
}

Point Point :: operator- (const Point& p) {
	Point ret (x - p.x, y - p.y, z - p.z);
	return ret;
}

float Point :: operator* (const Point& p) {
	return x * p.x + y * p.y + z * p.z;
}

Point Point :: operator* (const float& f) {
	Point ret (x * f, y * f, z * f);
	return ret;
}

Point Point :: operator/ (const float& f) {
	Point ret (x / f, y / f, z / f);
	return ret;
}

bool Point :: operator== (const Point& p) {
  Point tmp (x - p.x, y - p.y, z - p.z);
  return tmp.length() < 0.1;
}

Point Point :: normalize() {
	float length = sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
	if (length != 0) {
		Point ret (x / length, y / length, z / length);
		return ret;
	}
	Point ret;
	return ret;
}

Point Point :: crossProduct (Point p) {
	Point ret (y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
	return ret;
}

Point Point :: componentProduct (Point p) {
  Point ret (x * p.x, y * p.y, z * p.z);
  return ret;
}

float Point :: length() {
	return sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2));
}

void Point :: print() {
  cout << x << " " << y << " " << z << endl;
}
