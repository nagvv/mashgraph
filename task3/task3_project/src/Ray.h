/*
 * Ray.h
 *
 *  Created on: 23 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_RAY2_H_
#define SRC_RAY2_H_

#include <cmath>
#include "assimp/scene.h"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/glm.hpp"
#include "Scene.h"
#include "funcs.h"


using glm::vec3;
using glm::vec4;
using glm::distance;
using glm::normalize;
using glm::sqrt;
using glm::cross;

extern Scene myscene;
extern float*** wifibox;
extern int x_num,y_num,z_num;
extern float d;
extern vec3 box_min, box_max;
extern float maxpow;
extern ColorGradient heatmap;
extern float weps;

extern float alpha;
extern float reflectfine;
extern float ptfine;



vec3 camRay(vec3 pos, vec3 dir);
void wifiRay(vec3 pos, vec3 dir, int N, float fdist, float fine);//xperimental
void wifiRay2(vec3 pos, vec3 dir, float powerrrrrrrrrrrrrrr, int N);//reducing power for reflection

bool check_intersect(const vec3 pos,const vec3 dir, vec3 &dest, vec3 &norm);
bool check_intersect(const vec3 pos,const vec3 dir, float &dist, vec3 &norm, box box = myscene.rootbox);
bool check_intersect_ret(const vec3 pos,const vec3 dir, float &dist, vec3 &norm, unsigned int &meshid, box box = myscene.rootbox);
float* getBox(vec3 pos);

#endif /* SRC_RAY2_H_ */
