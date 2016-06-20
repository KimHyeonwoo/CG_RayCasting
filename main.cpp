#include "bmp2/EasyBMP.h"
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include <vector>

#define SIZE 500

Point camera (SIZE / 2, SIZE / 2, SIZE);
vector <Sphere> sphereList;
vector <Triangle> triangleList;

int raycast (int i, int j);

int main() {
  BMP out;
  out.SetSize(SIZE, SIZE);
	out.SetBitDepth(32);

  Point o1 (250, 250, 0);
  Point o2 (200, 250, 0);
  Point o3 (150, 250, 0);
  Sphere s1 (o1, 50);
  Sphere s2 (o2, 50);
  Sphere s3 (o3, 50);
  sphereList.push_back(s1); 
  sphereList.push_back(s2); 
  sphereList.push_back(s3); 

  Point p1 (300, 250, 0);
  Point p2 (400, 250, 0);
  Point p3 (400, 350, 0);
  Triangle t1 (p1, p2, p3);
  triangleList.push_back(t1);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      int minShapeIndex = raycast (i, j);
//      cout << i << " " << j << " " << minShapeIndex << endl;
      if (minShapeIndex == -1) { // No Hit
        out (i, j)->Red = 0;
        out (i, j)->Green = 0;
        out (i, j)->Blue = 0;
        out (i, j)->Alpha = 0;
      } else if (minShapeIndex == 0) {
        out (i, j)->Red = 255;
        out (i, j)->Green = 0;
        out (i, j)->Blue = 0;
        out (i, j)->Alpha = 255;
      } else if (minShapeIndex == 1) {
        out (i, j)->Red = 0;
        out (i, j)->Green = 255;
        out (i, j)->Blue = 0;
        out (i, j)->Alpha = 255;
      } else if (minShapeIndex == 2) {
        out (i, j)->Red = 0;
        out (i, j)->Green = 0;
        out (i, j)->Blue = 255;
        out (i, j)->Alpha = 255;
      } else if (minShapeIndex == 3) {
        out (i, j)->Red = 255;
        out (i, j)->Green = 255;
        out (i, j)->Blue = 0;
        out (i, j)->Alpha = 255;
      }
    }
  }
  
  out.WriteToFile ("hw5.bmp");
}

int raycast(int i, int j) {
  Point target (i, j, 0);
  Ray r (camera, target);
  int minShapeIndex = -1;
  float minLength = 10000000;

  for (int k = 0; k < sphereList.size(); k++) {
    Point tmp = sphereList[k].raycastHit (r, camera); 
    if (!(tmp == camera)) {
     // cout << i << " " << j << " ";
      tmp.print();
      if (minLength > (camera - tmp).length()) {
        minLength = (camera - tmp).length();
        minShapeIndex = k;
      }
    }
  }

  for (int k = 0; k < triangleList.size(); k++) {
    Point tmp = triangleList[k].raycastHit (r, camera);
    if (!(tmp == camera)) {
      if (minLength > (camera - tmp).length()) {
        minLength = (camera - tmp).length();
        minShapeIndex = k + sphereList.size();
      }
    }
  }

  return minShapeIndex;
  
}
