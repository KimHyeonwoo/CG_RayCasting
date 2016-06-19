#ifndef POINT_H
#define POINT_H
#include <iostream>

using namespace std;

class Point {
	public:
		float x, y, z;
		Point ();
		Point (float x, float y, float z);
		Point operator+ (const Point& p);
		Point operator- (const Point& p);
		float operator* (const Point& p);
		Point operator* (const float& f);
		Point operator/ (const float& f);
    bool operator== (const Point& p);
    Point normalize ();
		Point crossProduct (Point p);
		float length();
    void print();
};

#endif
