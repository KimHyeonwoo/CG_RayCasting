#include "bmp2/EasyBMP.h"
#include "point.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include <vector>

#define SIZE 1000

Point camera (SIZE / 2, SIZE / 2, SIZE);
vector <Sphere> sphereList;
vector <Triangle> triangleList;
vector <Light> lightList;

Point raycast (Point source, Point target, int depth);
void drawRect (Point p1, Point p2, Point p3, Point p4, Material mat);
float shadowRay (Point hit, Point light);

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

  Point lightPos (500, -450, -500);
  Point lightColor (1, 1, 1);
  Light l1 (lightPos, lightColor);
  lightList.push_back (l1);
  Point lightPos2 (250, 500, 0);
  Light l2 (lightPos2, lightColor);
  //lightList.push_back (l2);

  Point o1 (500, 650, -1000);
  Point o2 (200, 250, 0);
  Point o3 (150, 250, 0);
  Material m1 (CYAN, CYAN, CYAN, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material m2 (GREEN, GREEN, GREEN, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material m3 (BLUE, BLUE, BLUE, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Sphere s1 (o1, 100, m1);
  Sphere s2 (o2, 50, m2);
  Sphere s3 (o3, 50, m3);
  sphereList.push_back(s1); 
//  sphereList.push_back(s2); 
//  sphereList.push_back(s3); 

  Point p1 (-700, -700, -2900);
  Point p2 (-700, 1700, -2900);
  Point p3 (1700, 1700, -2900);
  Point p4 (1700, -700, -2900);
  Point p5 (-700, -700, -500);
  Point p6 (-700, 1700, -500);
  Point p7 (1700, 1700, -500);
  Point p8 (1700, -700, -500);
  Material m4 (WHITE, WHITE, WHITE, 0, 0.5, 0.1, 0.4, 0.5, 100, 1);
  drawRect (p1, p2, p3, p4, m4);
  drawRect (p1, p4, p8, p5, m4);
  drawRect (p3, p7, p8, p4, m4);
  drawRect (p2, p6, p7, p3, m4);
  drawRect (p1, p5, p6, p2, m4);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      Point target (i, j, 0);
      Point tmp = raycast (camera, target, 4);
      float max = tmp.x;
      if (max < tmp.y) max = tmp.y;
      if (max < tmp.z) max = tmp.z;
      if (max > 1) {
        out (i, j)->Red = tmp.x / max * 255;
        out (i, j)->Green = tmp.y / max * 255;
        out (i, j)->Blue = tmp.z / max * 255;
        out (i, j)->Alpha = 255;
      } else {
        out (i, j)->Red = tmp.x * 255;
        out (i, j)->Green = tmp.y * 255;
        out (i, j)->Blue = tmp.z * 255;
        out (i, j)->Alpha = 255;
      }
    }
  }
  out.WriteToFile ("hw5.bmp");
}

Point raycast(Point source, Point target, int depth) {
  if (depth == 0) {
    Point ret;
    return ret;
  }

  Ray r (source, target);
  int minShapeIndex = -1;
  float minLength = 10000000;

  for (int k = 0; k < sphereList.size(); k++) {
    Point tmp = sphereList[k].raycastHit (r, source); 
    if (!(tmp == source)) {
      if (minLength > (source - tmp).length()) {
        minLength = (source - tmp).length();
        minShapeIndex = k;
      }
    }
  }

  for (int k = 0; k < triangleList.size(); k++) {
    Point tmp = triangleList[k].raycastHit (r, source);
    if (!(tmp == source)) {
      if (minLength > (source - tmp).length()) {
        minLength = (source - tmp).length();
        minShapeIndex = k + sphereList.size();
      }
    }
  }

  if (minShapeIndex == -1) {
    Point ret;
    return ret;
  }

  Point hit = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].raycastHit (r, source) : sphereList[minShapeIndex].raycastHit (r, source);
  Point normal = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].getNormal (hit) : sphereList[minShapeIndex].getNormal (hit);
  Point viewNormal = (camera - hit).normalize();
  Material mat = minShapeIndex >= sphereList.size() ? triangleList[minShapeIndex - sphereList.size()].mat : sphereList[minShapeIndex].mat;
  
  Point ret = mat.ambient * mat.ambientConst;

  for (int k = 0; k < lightList.size(); k++) {
    Point lightNormal = (lightList[k].position - hit).normalize();
    Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (mat.diffuse) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point reflectNormal = normal * 2 * (lightNormal * normal) - lightNormal;
    // Point viewNormal = (camera - hit).normalize();
    Point specularLight = lightList[k].color.componentProduct (mat.specular) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
    float rayContribution = shadowRay (hit, lightList[k].position);
    ret = ret + (diffuseLight + specularLight) * rayContribution;
    /*
    if (rayContribution == 0.0) {
      cout << "cord ";
      cout << i << " " << j << " " << k << endl;
    }
    */
  }

  Point l = hit - source;
  Point reflectTarget = hit + l - normal * 2 * (l * normal);
  Point reflectLight = raycast (hit, reflectTarget, depth - 1);

  return ret * (1 - mat.alpha) * (1 - mat.beta) + reflectLight * (1 - mat.alpha) * mat.beta ;
  
}

float shadowRay (Point hit, Point light) {
  Ray shadow (hit, light);
  int count = 0;
  float distance = (light - hit).length();
  for (int i = 0; i < sphereList.size(); i++) {
    Point tmp = sphereList[i].raycastHit (shadow, hit);
    if (!(tmp == hit)) {
      if ((tmp - hit).length() < distance) {
        /*
        cout << "sphere hit ";
        hit.print();
        cout << "sphere tmp ";
        tmp.print();
        cout << "sphere dist " << (tmp - hit).length() << endl;
        */
        count++;
      }
    }
  }
  for (int i = 0; i <triangleList.size(); i++) {
    Point tmp = triangleList[i].raycastHit (shadow, hit);
    if (!(tmp == hit)) {
      if ((tmp - hit).length() < distance) {
        /*
        cout << "Triangle hit ";
        hit.print();
        cout << "Triangle tmp ";
        tmp.print();
        cout << "Triangle dist " << (tmp - hit).length() << endl;
        */
        count++;
      }
    }
  }
  return count == 0 ? 1.0 : 0.0;
}

void drawRect (Point p1, Point p2, Point p3, Point p4, Material mat) {
  Triangle t1 (p1, p3, p2, mat);
  Triangle t2 (p3, p1, p4, mat);
  triangleList.push_back (t1);
  triangleList.push_back (t2);
}
