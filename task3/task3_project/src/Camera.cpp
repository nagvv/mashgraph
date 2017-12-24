/*
 * Camera.cpp
 *
 *  Created on: 23 нояб. 2017 г.
 *      Author: Алексей
 */

#include "Camera.h"



Camera::Camera(vec3 pos, vec3 target, int x_res, int y_res, float fow)
{
	Camera::pos=pos;
	Camera::target=target;
	dir=target-pos;
	if(length(dir)==0)
		dir=vec3(1,0,0);
	else
		dir=normalize(dir);

	Camera::x_res=x_res;
	Camera::y_res=y_res;
	Camera::fow=fow;
	right = cross( dir, vec3(0,1,0));
	if(length(right)==0)
		right=vec3(1,0,0);
	else
		right=normalize(right);
	up=cross(dir,right);
}

shared_ptr<BMP> Camera::take()
{
	shared_ptr<BMP> out = make_shared<BMP>();
	out->SetSize(x_res, y_res);
	out->SetBitDepth(24);

	RGBApixel pixel;
	vec3 pix;

	vec3 cright, cup;
	cright=right*tan(radians(fow/2));
	cup=up*(static_cast<float>(y_res)/x_res)*length(cright);

	float cx=x_res/2, cy=y_res/2; //glm ne perevarivaet int\uint

	#pragma omp parallel for private(pixel,pix)
	for(unsigned int x=0; x<x_res; ++x)
		for(unsigned int y=0; y<y_res; ++y)
		{
			pix = camRay(pos,normalize(dir + cright*(x-cx)/cx + cup*(y-cy)/cy));
			pixel.Red=pix.r;
			pixel.Green=pix.g;
			pixel.Blue=pix.b;
			out->SetPixel(x,y,pixel);
		}


	return out;
}


