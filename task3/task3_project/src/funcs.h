/*
 * funcs.h
 *
 *  Created on: 23 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_FUNCS_H_
#define SRC_FUNCS_H_

#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "glm/glm.hpp"
#include "assimp/scene.h"
#include "glm/gtc/random.hpp"
#include "easyBMP/EasyBMP.h"


extern const aiScene* scene;
extern float*** wifibox;
extern int x_num,y_num,z_num;
extern float weps;

float dbmtomw(float power);

float mwtodbm(float lpower);

//return max vector for wifi box
glm::vec3 max_vertex(aiNode *node);

//return min vector for wifi box
glm::vec3 min_vertex(aiNode *node);

void Boxfilter();
void Gaussfilter();
glm::vec3 findoptimal(int iters, float minpow, float power, int wnum, int wrefnum, float step, float ostanov, float gamma, bool visual, int Res, glm::vec3 pos, bool mypos, float &op, int num=0);

void gray_world(std::shared_ptr<BMP> img);
void f_unsharp(std::shared_ptr<BMP> &img);
void autocon(std::shared_ptr<BMP> img, float fraction);

/*
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
*/
//somewhat changed by me, eee
bool HitBoundingBox(glm::vec3 minB, glm::vec3 maxB, glm::vec3 origin, glm::vec3 dir, glm::vec3 &coord);


/*
ColorGradient
by Andrew Noske
from www.andrewnoske.com
 */
//somewhat changed by me, yup

using std::vector;
using std::max;
class ColorGradient
{
private:
  struct ColorPoint  // Internal class used to store colors at different points in the gradient.
  {
    float r,g,b;      // Red, green and blue values of our color.
    float val;        // Position of our color along the gradient (between 0 and 1).
    ColorPoint(float red, float green, float blue, float value)
      : r(red), g(green), b(blue), val(value) {}
  };
  vector<ColorPoint> color;      // An array of color points in ascending value.

public:
  //-- Default constructor:
  ColorGradient();

  //-- Inserts a new color point into its correct position:
  void addColorPoint(float red, float green, float blue, float value);

  //-- Inserts a new color point into its correct position:
  void clearGradient();

  //-- Places a 5 color heapmap gradient into the "color" vector:
  void createDefaultHeatMapGradient();

  //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
  //-- values representing that position in the gradient.
  void getColorAtValue(const float value, float &red, float &green, float &blue);
};


#endif /* SRC_FUNCS_H_ */
