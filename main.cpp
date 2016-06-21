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
#define SUPERSAMPLE 1
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

BMP checker;
BMP baseball;

int maxInt(int a, int b) {
  return a > b ? a : b;
}

int main() {
  BMP out;
  out.SetSize(SIZE, SIZE);
	out.SetBitDepth(32);
  checker.ReadFromFile("texture.bmp");
  baseball.ReadFromFile("baseball.bmp");

  Point lightPos (0.5 * SIZE, -0.45 * SIZE, -0.5 * SIZE);
  Point lightColor (2, 2, 2);
  Light l1 (lightPos, lightColor);
  lightList.push_back (l1);
  
  vector <vector <Point> > colors;

  for (int i = 0; i < SIZE; i++) {
    vector <Point> tmp;
    for (int j = 0; j < SIZE; j++) {
      Point tmpPoint;
      tmp.push_back (tmpPoint);
    }
    colors.push_back (tmp);
  } 

  for (int t = 0; t <= 100; t++) {
    sphereList.clear();
    Point o1 (-2.0 * SIZE + sqrt (t / 1000.0) * 11.0 * SIZE, -0.25 * SIZE + pow (t - 50, 2) / 7000 * SIZE, -1.5 * SIZE);
    Material sphereMat (MAGENTA, MAGENTA, MAGENTA, 0, 0, 0.3, 0.4, 0.3, 50, 1);
    Sphere s1 (o1, 0.5 * SIZE, sphereMat);
    sphereList.push_back(s1); 

//    Point p1 (-2.5 * SIZE, -2.5 * SIZE, -3.0 * SIZE);
//    Point p2 (-2.5 * SIZE, 3.5 * SIZE, -3.0 * SIZE);
//    Point p3 (3.5 * SIZE, 3.5 * SIZE, -3.0 * SIZE);
//    Point p4 (3.5 * SIZE, -2.5 * SIZE, -3.0 * SIZE);
//    Material planeMat (WHITE, WHITE, WHITE, 0, 0.5, 0.1, 0.3, 0.5, 100, 1);
//    drawRect (p1, p2, p3, p4, planeMat);

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
          colors[i][j].x += result.x / max * 255 * maxInt (t - 90, 1);
          colors[i][j].y += result.y / max * 255 * maxInt (t - 90, 1);
          colors[i][j].z += result.z / max * 255 * maxInt (t - 90, 1);
        } else {
          colors[i][j].x += result.x * 255 * maxInt (t - 90, 1);
          colors[i][j].y += result.y * 255 * maxInt (t - 90, 1);
          colors[i][j].z += result.z * 255 * maxInt (t - 90, 1);
        }
      }
    }
    cout << t << endl;
  }
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      out (i, j)->Red = colors[i][j].x / 146.0;
      out (i, j)->Green = colors[i][j].y / 146.0;
      out (i, j)->Blue = colors[i][j].z / 146.0;
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
  
//  Point ret = mat.ambient * mat.ambientConst; 
  Point tmpAmb = getAmbient (minShapeIndex, hit);
  Point ret = tmpAmb * mat.ambientConst;

  for (int k = 0; k < lightList.size(); k++) {
    Point lightNormal = (lightList[k].position - hit).normalize();
    //Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (mat.diffuse) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point diffuseLight = (normal * lightNormal) > 0 ? lightList[k].color.componentProduct (tmpAmb) * mat.diffuseConst * (normal * lightNormal) : lightList[k].color * 0;
    Point reflectNormal = normal * 2 * (lightNormal * normal) - lightNormal;
    //Point specularLight = lightList[k].color.componentProduct (mat.specular) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
    Point specularLight = lightList[k].color.componentProduct (tmpAmb) * mat.specularConst * pow (reflectNormal * viewNormal, mat.gloss);
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
    //u = u > 0.2 ? u - 0.2 : u + 0.8;
    //v = v > 0.8 ? 1 : v * 1.25;
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


