/*
 * Wifi.h
 *
 *  Created on: 25 нояб. 2017 г.
 *      Author: Алексей
 */

#ifndef SRC_WIFI_H_
#define SRC_WIFI_H_


#include "Ray.h"
#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"

using glm::vec3;
using glm::sphericalRand;

extern bool refmode;

class Wifi
{
private:
	vec3 pos;
	float pow;
public:
	Wifi(vec3 pos, float pow);
	void start(unsigned int num, unsigned int N);

};

#endif /* SRC_WIFI_H_ */
