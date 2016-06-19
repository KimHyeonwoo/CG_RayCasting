#include "sphere.h"

Point Sphere :: raycastHit (Ray ray) {
	float b = ray.dir * (origin - ray.origin);
	float c = (origin - ray.origin) * (origin - ray.origin) - radius * radius;
	
	if (c > 0) {
		if (b - sqrt (c) > 0) {
			return ray.origin + ray.dir * (b - sqrt (c));
		}

		else if (b + sqrt (c) > 0) {
			return ray.origin + ray.dir * (b + sqrt (c));
		}

		else {
			Point ret;
			return ret;
		}
	} 
	
	else if (c == 0) {
		if (b > 0) {
			return ray.origin + ray.dir * b;
		}

		else {
			Point ret;
			return ret;
		}
	} 

	else {
		Point ret;
		return ret;
	}
}

Point Sphere :: getNormal (Point p) {
	return (p - origin).normalize();
}
