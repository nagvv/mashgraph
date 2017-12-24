#include "Ray.h"


bool check_intersect(const vec3 pos,const vec3 dir, vec3 &dest, vec3 &norm)
{
	float dist=0;
	bool is = check_intersect(pos, dir, dist, norm);
	if(is)
	{
		dest=pos + normalize(dir)*dist;
		return true;
	}
	else
		return false;
}


bool check_intersect(const vec3 pos,const vec3 dir, float &dist, vec3 &norm, box box)
{

	bool b=true;//is dist not set yet

	vec3 coord;

	if(!HitBoundingBox(box.min, box.max, pos, dir, coord))
	{
		return false;
	}

	if(box.has_sb)
		for(unsigned int i=0; i<box.subboxes.size(); ++i)
		{
			vec3 hit;
			float tdist;
			vec3 tnorm;
			bool is = check_intersect(pos,dir,tdist,tnorm, box.subboxes[i]);
			if(is && (b || tdist<dist))
			{
				dist=tdist;
				norm=tnorm;
				b=false;
			}
		}

	if(box.has_mesh)
	{
		int num_indicies = myscene.meshes[box.meshid].indicies.size();
		for( int k=0; k < num_indicies; k+=3 )
		{
			vec3 v1 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k]].pos;
			vec3 v2 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k+1]].pos;
			vec3 v3 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k+2]].pos;
			vec3 tmp;
			bool is = glm::intersectRayTriangle<vec3>(pos,dir,v1,v2,v3, tmp);
			if(is && (tmp.z < dist || b))
			{
				dist=tmp.z;
				norm=normalize(cross(v2-v1,v3-v2));
				b=false;
			}
		}
	}


	if(dot(norm,dir)>0)
		norm*=-1;

	if(b)
		return false;
	else
		return true;
}

bool check_intersect_ret(const vec3 pos,const vec3 dir, float &dist, vec3 &norm, unsigned int &meshid, box box)
{
	bool b=true;//is dist not set yet

	vec3 coord;

	if(!HitBoundingBox(box.min, box.max, pos, dir, coord))
		return false;


	if(box.has_sb)
		for(unsigned int i=0; i<box.subboxes.size(); ++i)
		{
			float tdist;
			vec3 tnorm;
			unsigned int tmeshid;
			bool is = check_intersect_ret(pos,dir,tdist,tnorm, tmeshid, box.subboxes[i]);
			if(is && (b || tdist<dist))
			{
				dist=tdist;
				norm=tnorm;
				meshid=tmeshid;
				b=false;
			}
		}

	if(box.has_mesh)
	{
		int num_indicies = myscene.meshes[box.meshid].indicies.size();
		for( int k=0; k < num_indicies; k+=3 )
		{
			vec3 v1 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k]].pos;
			vec3 v2 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k+1]].pos;
			vec3 v3 = myscene.meshes[box.meshid].vertices[myscene.meshes[box.meshid].indicies[k+2]].pos;
			vec3 tmp;
			bool is = glm::intersectRayTriangle<vec3>(pos,dir,v1,v2,v3, tmp);
			if(is && (tmp.z < dist || b))
			{
				dist=tmp.z;
				norm=normalize(cross(v2-v1,v3-v2));
				meshid=box.meshid;
				b=false;
			}
		}
	}

	if(dot(norm,dir)>0)
		norm*=-1;

	if(b)
		return false;
	else
		return true;
}

float* getBox(vec3 pos)
{
	vec3 tmp=pos-box_min;
	tmp/=d;
	int x=tmp.x, y=tmp.y, z=tmp.z;//okruglenie vniz
	if(x>=x_num || x<0 || y>=y_num || y<0 || z>=z_num || z<0)
		return nullptr;
	return &wifibox[x][y][z];
}

vec3 camRay(vec3 pos, vec3 dir)
{
	float lpower=225;
	float light=30;
	//checking intersects
	float dist=0;
	unsigned int meshid;
	vec4 color4;
	vec3 color;
	vec3 norm;
	if(check_intersect_ret(pos, dir, dist, norm, meshid))
	{
		color4=myscene.meshes[meshid].color;
		float cos=-dot(dir, norm);
		float tlight=lpower*cos;
		if(tlight>0)
			light+=tlight;
	}
	else
	{
		color4=vec4(0,0,0,1);
	}
	color=vec3(color4.r,color4.g,color4.b);
	color*=light;

	if(color4.a<1)
	{
		vec3 newpos=pos+dir*(dist+0.00001f);//4tobu luch proletal, a ne ve4no zastral v polygone
		vec3 acolor = camRay(newpos,dir);
		color=color4.a*color + (1 - color4.a)*acolor;
	}


	//vec3 ret=vec3(255,255,255);
	vec3 st=pos+dir*dist;
	float step=d/2;
	float cur=0;
	float *box;
	vec3 fog_color;
	while(cur<dist)
	{
		box = getBox(st-dir*cur);
		if(box==nullptr)
			break;
		float pow= (mwtodbm(*box) - mwtodbm(weps) )/ (mwtodbm(maxpow) - mwtodbm(weps));
		//float pow=(*box)/maxpow;
		if (pow<0)
			pow=0;
		heatmap.getColorAtValue( pow,fog_color.r, fog_color.g, fog_color.b);
		//heatmap.getColorAtValue(( dbmtomw(*box))/ dbmtomw(maxpow) ,fog_color.r, fog_color.g, fog_color.b);
		fog_color*=255;
		color=alpha*fog_color+(1-alpha)*color;
		cur+=step;
	}
	return color;//rgb
}

inline float getPower2(float power, float dist)
{
	return power/(1+dist*dist);
}



inline float getMaxDist2(float power)
{
	if(power<=weps)
		return 0;
	else
		return sqrt(power/weps-1);
}

inline float getPower(float dist)
{
	return maxpow/(1+dist*dist);
}

inline float getMaxDist(float fine)
{
	if(fine-maxpow>weps)
		return 0;
	else
		return sqrt(maxpow/(weps+fine)-1);
}

void wifiRay(vec3 pos, vec3 dir, int N, float fdist, float fine)
{
	float dist;
	vec3 norm=vec3(0,1,0);
	bool reflected=false;//sobsno ne tolko otrazilsa, no i proletel
	bool prolet=false;
	float maxdist=getMaxDist(fine)-fdist;
	unsigned int meshid;
	if(!check_intersect_ret(pos, dir, dist,norm, meshid))
	{
		dist=maxdist;
	}
	else
		reflected=true;

	if(myscene.meshes[meshid].transparent)
		prolet=true;

	if(dist>maxdist)
	{
		dist=maxdist;
		reflected=false;
	}

	float cur=0;
	float step=d;
	float np;
	float *box;
	while(cur<=dist)
	{
		box = getBox(pos+dir*cur);
		if(box==nullptr)
			break;
		np=getPower(cur+fdist)-fine;
		if(np > *box)
			*box=np;
		cur+=step;
	}
	if(reflected)
	{
		if(prolet)
		{
			fine+=ptfine;
			vec3 newpos=pos+dir*(dist+0.00001f);
			if(getBox(newpos)!=nullptr && N>0 && getMaxDist(fine)-fdist-dist >weps)
				wifiRay(newpos, dir, N-1, fdist+dist, fine);
		}
		else
		{
			fine+=reflectfine;
			vec3 newpos=pos+dir*(dist-0.00001f);
			vec3 newdir=normalize(glm::reflect(dir,norm));//dir - 2*dot(dir, norm) *norm;
			if(getBox(newpos)!=nullptr && N>0 && getMaxDist(fine)-fdist-dist >weps)
				wifiRay(newpos, newdir, N-1, fdist+dist, fine);
		}
	}
}




void wifiRay2(vec3 pos, vec3 dir, float pow, int N)
{
	float dist;
	vec3 norm=vec3(0,1,0);
	bool reflected=false;
	bool prolet=false;
	float maxdist=getMaxDist2(pow);
	unsigned int meshid;
	if(!check_intersect_ret(pos, dir, dist,norm, meshid))
	{
		dist=maxdist;
	}
	else
		reflected=true;

	if(myscene.meshes[meshid].transparent)
		prolet=true;

	if(dist>maxdist)
	{
		dist=maxdist;
		reflected=false;
	}
	float cur=0;
	float step=d/2;
	float np;
	float *box;
	while(cur<=dist)
	{
		box = getBox(pos+dir*cur);
		if(box==nullptr)
			break;
		np=getPower2(pow, cur);
		if(np > *box)
			*box=np;
		cur+=step;
	}
	if(reflected)
	{
		if(prolet)
		{
			vec3 newpos=pos+dir*(dist+0.00001f);
			if(getBox(newpos)!=nullptr && N>0 && pow-ptfine>weps)
				wifiRay2(newpos, dir, getPower2(pow, dist)-ptfine, N-1);
		}
		else
		{
			vec3 newpos=pos+dir*(dist-0.00001f);
			vec3 newdir=normalize(glm::reflect(dir,norm));//dir - 2*dot(dir, norm) *norm;
			if(getBox(newpos)!=nullptr && N>0 && pow-reflectfine>weps)
				wifiRay2(newpos, newdir, getPower2(pow, dist)-reflectfine, N-1);
		}

	}
}

