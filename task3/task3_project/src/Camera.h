/*
 * Camera.h
 *
 *  Created on: 23 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#include <memory>
#include "glm/glm.hpp"
#include "easyBMP/EasyBMP.h"
#include "Ray.h"
#include <omp.h>

using glm::vec3;
using glm::normalize;
using std::shared_ptr;
using std::make_shared;
using glm::cross;
using glm::equal;
using glm::length;
using glm::tan;
using glm::radians;




class Camera
{
private:
	vec3 pos;
	vec3 target;
	vec3 dir;//nomralize(target-pos)
	unsigned int x_res, y_res;
	float fow;
	vec3 up;
	vec3 right;
public:
	Camera(vec3 pos, vec3 target, int x_res, int y_res, float fow);
	shared_ptr<BMP> take();

};



#endif /* SRC_CAMERA_H_ */
