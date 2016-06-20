#include "bmp2/EasyBMP.h"
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include <vector>

#define SIZE 500

Point camera (SIZE / 2, SIZE / 2, SIZE);
vector <Sphere> sphereList;
vector <Triangle> triangleList;
vector <Light> lightList;

Point raycast (int i, int j);

Point BLACK (0, 0, 0);
Point RED (1, 0, 0);
Point GREEN (0, 1, 0);
Point BLUE (0, 0, 1);
Point YELLOW (1, 1, 0);
Point CYAN (0, 1, 1);
Point MAGENTA (1, 0, 1);
Point WHITE (1, 1, 1);

int main() {
  BMP out;
  out.SetSize(SIZE, SIZE);
	out.SetBitDepth(32);

  Point lightPos (250, 0, 0);
  Point lightColor (1, 1, 1);
  Light l1 (lightPos, lightColor);
  lightList.push_back (l1);

  Point o1 (250, 250, 0);
  Point o2 (200, 250, 0);
  Point o3 (150, 250, 0);
  Material m1 (CYAN, CYAN, CYAN, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material m2 (GREEN, GREEN, GREEN, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material m3 (BLUE, BLUE, BLUE, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Sphere s1 (o1, 50, m1);
  Sphere s2 (o2, 50, m2);
  Sphere s3 (o3, 50, m3);
  sphereList.push_back(s1); 
  sphereList.push_back(s2); 
  sphereList.push_back(s3); 

  Point p1 (300, 250, 0);
  Point p2 (400, 250, 0);
  Point p3 (400, 350, 0);
  Material m4 (YELLOW, YELLOW, YELLOW, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Triangle t1 (p1, p2, p3, m4);
  triangleList.push_back(t1);
/*
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
*/

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      Point tmp = raycast (i, j);
      out (i, j)->Red = tmp.x * 255;
      out (i, j)->Green = tmp.y * 255;
      out (i, j)->Blue = tmp.z * 255;
      out (i, j)->Alpha = 255;
    }
  }

  out.WriteToFile ("hw5.bmp");
}

Point raycast(int i, int j) {
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

  if (minShapeIndex == -1) {
    Point ret;
    return ret;
  }

  Point hit = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].raycastHit (r, camera) : sphereList[minShapeIndex].raycastHit (r, camera);
  Point normal = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].getNormal (hit) : sphereList[minShapeIndex].getNormal (hit);
  Material mat = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].mat : sphereList[minShapeIndex].mat;
  
  Point ret;

  for (int k = 0; k < lightList.size(); k++) {
    Point lightNormal = (lightList[k].position - hit).normalize();
    Point ambientLight = mat.ambient * mat.ambientConst;
    Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (mat.diffuse) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point reflectNormal = normal * 2 * (lightNormal * normal) - lightNormal;
    Point viewNormal = (camera - hit).normalize();
    Point specularLight = lightList[k].color.componentProduct (mat.specular) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
    ret = ret + ambientLight + diffuseLight + specularLight;
  }

  return ret;
  
}


