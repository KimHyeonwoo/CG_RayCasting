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
#define SUPERSAMPLE 4
#define PI 3.141592

Point camera (SIZE / 2, SIZE / 2, SIZE / 2);
vector <Sphere> sphereList;
vector <Triangle> triangleList;
vector <Light> lightList;

Point raycast (Point source, Point target, int depth);
void drawRect (Point p1, Point p2, Point p3, Point p4, Material mat);
float shadowRay (Point hit, Point light);
Point getAmbient (int minShapeIndex, Point hit);
Point getDiffuse (int minShapeIndex, Point hit);
Point getSpecular (int minShapeIndex, Point hit);

Point BLACK (0, 0, 0);
Point RED (1, 0, 0);
Point GREEN (0, 1, 0);
Point BLUE (0, 0, 1);
Point YELLOW (1, 1, 0);
Point CYAN (0, 1, 1);
Point MAGENTA (1, 0, 1);
Point WHITE (1, 1, 1);
Point PINK (255 / 255.0, 186 / 255.0, 210 / 255.0);

BMP checker;
BMP baseball;

int main() {
  BMP out;
  out.SetSize(SIZE, SIZE);
	out.SetBitDepth(32);
  checker.ReadFromFile("texture.bmp");
  baseball.ReadFromFile("baseball.bmp");

  Point lightPos (0, -0.45 * SIZE, -1.5 * SIZE);
  Point lightColor (0.7, 0.7, 0.7);
  Light l1 (lightPos, lightColor);
  lightList.push_back (l1);
  Point lightPos2 (SIZE, -0.45 * SIZE, -1.5 * SIZE);
  Light l2 (lightPos2, lightColor);
  lightList.push_back (l2);

  Point o1 (0.5 * SIZE, 1.55 * SIZE, -1.5 * SIZE);
  //Point o1 (0.5 * SIZE, 0.75 * SIZE, -1.5 * SIZE);
  Point o2 (0.5 * SIZE, 0.75 * SIZE, -1.5 * SIZE);
  Point o3 (0.6 * SIZE, 0.65 * SIZE, -1.5 * SIZE);
  Point o4 (0.1 * SIZE, 0.5 * SIZE, -1.2 * SIZE);
  Material m1 (CYAN, CYAN, CYAN, 1.0, 1.0, 0.1, 0.4, 0.5, 50, 0.8);
  Material m2 (WHITE, WHITE, WHITE, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material m3 (PINK, PINK, PINK, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Material sphereMat (MAGENTA, MAGENTA, MAGENTA, 0, 0, 0.1, 0.4, 0.5, 50, 1);
  Sphere s1 (o1, 0.1 * SIZE, sphereMat);
  Sphere s2 (o2, 0.1 * SIZE, m2);
  Sphere s3 (o3, 0.1 * SIZE, m3);
  Sphere s4 (o4, 0.3 * SIZE, m1);
  sphereList.push_back(s1); 
  sphereList.push_back(s4);
//  sphereList.push_back(s2); 
//  sphereList.push_back(s3); 

  Point p1 (-0.7 * SIZE, -0.7 * SIZE, -2.9 * SIZE);
  Point p2 (1.7 * SIZE, -0.7 * SIZE, -2.9 * SIZE);
  Point p3 (1.7 * SIZE, 1.7 * SIZE, -2.9 * SIZE);
  Point p4 (-0.7 * SIZE, 1.7 * SIZE, -2.9 * SIZE);
  Point p5 (-0.7 * SIZE, -0.7 * SIZE, 1.0 * SIZE);
  Point p6 (1.7 * SIZE, -0.7 * SIZE, 1.0 * SIZE);
  Point p7 (1.7 * SIZE, 1.7 * SIZE, 1.0 * SIZE);
  Point p8 (-0.7 * SIZE, 1.7 * SIZE, 1.0 * SIZE);
  Material m4 (WHITE, WHITE, WHITE, 0, 0, 0.1, 0.4, 0.5, 100, 1);
  Material m5 (WHITE, WHITE, WHITE, 0, 0.2, 0.1, 0.4, 0.5, 100, 1);
  drawRect (p1, p2, p3, p4, m4); // Front Face
  drawRect (p1, p4, p8, p5, m3); // Top Face
  drawRect (p3, p7, p8, p4, m2); // Right Face
  drawRect (p2, p6, p7, p3, m3); // Bottom Face
  drawRect (p1, p5, p6, p2, m5); // Left Face
  drawRect (p8, p7, p6, p5, m4); // Back Face

  Point glass1 (0.5 * SIZE - SIZE / sqrt(3), 1.1 * SIZE, -1.0 * SIZE);
  Point glass2 (0.5 * SIZE, 1.1 * SIZE, -2.0 * SIZE);
  Point glass3 (0.5 * SIZE + SIZE / sqrt(3), 1.1 * SIZE, -1.0 * SIZE);
  Point glass4 (0.5 * SIZE - SIZE / sqrt(3), 1.7 * SIZE, -1.0 * SIZE);
  Point glass5 (0.5 * SIZE, 1.7 * SIZE, -2.0 * SIZE);
  Point glass6 (0.5 * SIZE + SIZE / sqrt(3), 1.7 * SIZE, -1.0 * SIZE);
  Material glassMat (YELLOW, YELLOW, YELLOW, 0, 0.85, 0.1, 0.4, 0.5, 100, 1);
  drawRect (glass1, glass2, glass5, glass4, glassMat);
  drawRect (glass2, glass3, glass6, glass5, glassMat);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      Point result;
      for (int k = 0; k < SUPERSAMPLE; k++) {
        for (int l = 0; l < SUPERSAMPLE; l++) {
          Point target (i + (double) k / SUPERSAMPLE, j + (double) l / SUPERSAMPLE, 0);
          result = result + raycast (camera, target, 6);
        }
      }
      result = result / pow (SUPERSAMPLE, 2);
      float max = result.x;
      if (max < result.y) max = result.y;
      if (max < result.z) max = result.z;
      if (max > 1) {
        out (i, j)->Red = result.x / max * 255;
        out (i, j)->Green = result.y / max * 255;
        out (i, j)->Blue = result.z / max * 255;
        out (i, j)->Alpha = 255;
      } else {
        out (i, j)->Red = result.x * 255;
        out (i, j)->Green = result.y * 255;
        out (i, j)->Blue = result.z * 255;
        out (i, j)->Alpha = 255;
      }
    }
    out.WriteToFile ("hw5.bmp");
    cout << i << endl;
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
  
//  Point ret = mat.ambient * mat.ambientConst;
  Point ret = getAmbient (minShapeIndex, hit) * mat.ambientConst;

  for (int k = 0; k < lightList.size(); k++) {
    Point lightNormal = (lightList[k].position - hit).normalize();
    //Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (mat.diffuse) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (getAmbient (minShapeIndex, hit)) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point reflectNormal = normal * 2 * (lightNormal * normal) - lightNormal;
    // Point viewNormal = (camera - hit).normalize();
    //Point specularLight = lightList[k].color.componentProduct (mat.specular) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
    Point specularLight = lightList[k].color.componentProduct (getAmbient (minShapeIndex, hit)) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
    float rayContribution = shadowRay (hit, lightList[k].position);
    ret = ret + (diffuseLight + specularLight) * rayContribution;
    /*
    if (rayContribution == 0.0) {
      cout << "cord ";
      cout << i << " " << j << " " << k << endl;
    }
    */
  }

  // Reflect
  Point l = (hit - source).normalize();
  Point reflectTarget = hit + l - normal * 2 * (l * normal);
  Point reflectLight = raycast (hit, reflectTarget, depth - 1);

  // Refract
  if (minShapeIndex < sphereList.size()) { // refract for sphere
    if (mat.n * mat.n - 1 + (l * normal) * (l * normal) < 0) return ret * (1 - mat.beta) + reflectLight * mat.beta;
    Point refractVector = l / mat.n - normal * ((l * normal) / mat.n + sqrt(mat.n * mat.n - 1 + (l * normal) * (l * normal)) / mat.n);
    Point refractVector2 = refractVector * 2 * (l * refractVector) - l;
    Point refractSource = hit - refractVector * 2 * ((hit - sphereList[minShapeIndex].origin) * refractVector);
    Point refractTarget = refractSource + refractVector2;
    Point refractLight = raycast (refractSource, refractTarget, depth - 1);
    return ret * (1 - mat.alpha) * (1 - mat.beta) + reflectLight * (1 - mat.alpha) * mat.beta + refractLight * mat.alpha;
  } else { // refract for triangle
    return ret * (1 - mat.beta) + reflectLight * mat.beta;
  }

  return ret * (1 - mat.alpha) * (1 - mat.beta) + reflectLight * (1 - mat.alpha) * mat.beta ;
  
}

float shadowRay (Point hit, Point light) {
  Point dx (SIZE / 20, 0, 0);
  Point dz (0, 0, SIZE / 20);
  int count = 0;
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      Ray shadow (hit, light + dx * i + dz * j);
      int tmpCount = 0;
      float distance = (light + dx * i + dz * j - hit).length();
      for (int k = 0; k < sphereList.size(); k++) {
        Point tmp = sphereList[k].raycastHit (shadow, hit);
        if (!(tmp == hit)) {
          if ((tmp - hit).length() < distance) {
            tmpCount++;
          }
        }
      }
      for (int k = 0; k < triangleList.size(); k++) {
        Point tmp = triangleList[k].raycastHit (shadow, hit);
        if (!(tmp == hit)) {
          if ((tmp - hit).length() < distance) {
            tmpCount++;
          }
        }
      }
      count = tmpCount == 0 ? count + 1 : count;
    }
  }
  return (double) count / 9.0;
}

void drawRect (Point p1, Point p2, Point p3, Point p4, Material mat) {
  Triangle t1 (p1, p2, p3, mat);
  Triangle t2 (p3, p4, p1, mat);
  triangleList.push_back (t1);
  triangleList.push_back (t2);
}

Point getAmbient (int minShapeIndex, Point hit) {
  if (minShapeIndex == 0) {
    Point n = (hit - sphereList[0].origin).normalize();
    float u = (atan2 (n.y, n.x) + PI) / (2 * PI);
    float v = (atan2 (sqrt (pow (n.x, 2) + pow (n.y, 2)), n.z) / PI);
    u = u > 0.2 ? u - 0.2 : u + 0.8;
    v = v > 0.8 ? 1 : v * 1.25;
    Point ret (baseball (u * 400, v * 200)->Red / 255.0, baseball (u * 400, v * 200)->Green / 255.0, baseball (u * 400, v * 200)->Blue / 255.0);
    return ret;
  }
  if (minShapeIndex == sphereList.size() || minShapeIndex == sphereList.size() + 1) {
    Point u = triangleList[0].p3 - triangleList[0].p2;
    Point v = triangleList[0].p1 - triangleList[0].p2;
    Point w = hit - triangleList[0].p2;
    float s1 = ((u * v) * (w * v) - (v * v) * (w * u)) / ((u * v) * (u * v) - (u * u) * (v * v));
    float s2 = ((u * v) * (w * u) - (u * u) * (w * v)) / ((u * v) * (u * v) - (u * u) * (v * v));
    Point ret (checker (s1 * 283, s2 * 283)->Red / 255.0, checker (s1 * 283, s2 * 283)->Green / 255.0, checker (s1 * 283, s2 * 283)->Blue / 255.0);
    return ret;
  }
  return minShapeIndex < sphereList.size() ? sphereList[minShapeIndex].mat.ambient : triangleList[minShapeIndex - sphereList.size()].mat.ambient;
}


