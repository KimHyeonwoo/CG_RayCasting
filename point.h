#ifndef POINT_H
#define POINT_H

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
		Point normalize ();
		Point crossProduct (Point p);
		float length();
};

#endif
