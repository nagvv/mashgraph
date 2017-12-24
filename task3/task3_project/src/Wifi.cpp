/*
 * Wifi.cpp
 *
 *  Created on: 25 нояб. 2017 г.
 *      Author: Алексей
 */

#include "Wifi.h"


Wifi::Wifi(vec3 position, float power)
{
	pos=position;
	pow=power;
}

void Wifi::start(unsigned int num, unsigned int N)
{
	vec3 dir(0,0,0);
	#pragma omp parallel for private(dir)
	for(unsigned int i=0; i<num; ++i)
	{
		dir = sphericalRand(1.0);
		if(refmode)
			wifiRay(pos, dir,N, 0, 0);
		else
			wifiRay2(pos,dir,pow,N);
	}
}
