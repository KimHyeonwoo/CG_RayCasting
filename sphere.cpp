#include "sphere.h"

Sphere :: Sphere (Point o, float r, Material mat) {
  origin = o;
  radius = r;
  this->mat = mat;
}

Point Sphere :: raycastHit (Ray ray, Point noHit) {
	float b = ray.dir * (origin - ray.origin);
	float c = (origin - ray.origin) * (origin - ray.origin) - radius * radius;

  //ray.dir.print();
  cout << b << " " << sqrt(c) << endl;

	if (pow (b, 2) - c > 0) {
		if (b - sqrt (pow (b, 2) - c) > 0) {
			return ray.origin + ray.dir * (b - sqrt (pow (b, 2) - c));
		}

		else if (b + sqrt (pow (b, 2) - c) > 0) {
			return ray.origin + ray.dir * (b + sqrt (pow (b, 2) - c));
		}

		else {
			return noHit;
		}
	} 
	
	else if (pow (b, 2) - c == 0) {
		if (b > 0) {
			return ray.origin + ray.dir * b;
		}

		else {
			return noHit;
		}
	} 

	else {
		return noHit;
	}
}

Point Sphere :: getNormal (Point p) {
	return (p - origin).normalize();
}
