#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <omp.h>
#include <string>
#include <fstream>
#include <cmath>
#include <array>

//assimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

//glm
#include "glm/glm.hpp"

//easyBMP
#include "easyBMP/EasyBMP.h"

//inih
#include "inih/INIReader.h"

//my headers
#include "funcs.h"
#include "Wifi.h"
#include "Camera.h"
#include "Scene.h"

using std::cout;
using std::endl;
using glm::vec3;
using std::string;
using std::ifstream;
using std::cbrt;
using std::array;


//scene
const aiScene* scene = nullptr;
Scene myscene;
ColorGradient heatmap;

//wifibox
float*** wifibox = nullptr;
int x_num,y_num,z_num;
float d;
vec3 box_min,box_max;
float maxpow;
float weps;

//default global parameters
float alpha=0.01;
float reflectfine=0.01;
float ptfine=0.01;
int wt_mode=1;
bool refmode=true;

bool importScene( const char* file)
{
	scene = aiImportFile(file,  aiProcess_Triangulate | aiProcess_FlipUVs|aiProcess_OptimizeMeshes);
	if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::"<< aiGetErrorString() << endl;
		return false;
	}
	return true;
}

int main(int argc, char** argv)
{

	//default parameters
	unsigned int xRes= 1920, yRes=1080;
	vec3 pos = vec3(10,10,5);
	vec3 target = vec3(0,0,0);
	unsigned int fow = 100;
	vec3 wpos = vec3(5.37,0.5,0);
	float power=20;
	int wifirays_num=100000;
	float wrefnum=8;
	int minboxres=40;
	//vec3 lamppos= vec3(0, 5, 0);
	bool usegauss=true;

	bool findopt=false;
	bool notopt=false;

	bool grayworld=false;
	bool unsharp=false;
	bool autocontrast=false;
	float fraction=0;


	//parsing arg list
	string f_obj, f_set;
	if( argc < 2 )
	{
		cout << "do: task <obj> [setting]" <<endl;
		return 0;
	}
	else if(argc == 2)
	{
		cout << "using default settings" << endl;
		f_obj=argv[1];
	}
	else
	{
		f_obj=argv[1];
		f_set=argv[2];
	}

	//reading parameters from config(ini) file
	if(argc>2)
	{
		INIReader ini(f_set);
		if(ini.ParseError() < 0)
		{
			cout << "cannot import settings" <<endl;
			return 1;
		}

		minboxres=ini.GetInteger("wifibox", "minboxres", minboxres);
		alpha=ini.GetReal("wifibox", "alpha", alpha);

		xRes=ini.GetInteger("camera", "xRes", xRes);
		yRes=ini.GetInteger("camera", "yRes", yRes);

		pos.x=ini.GetReal("camera", "cx", pos.x);
		pos.y=ini.GetReal("camera", "cy", pos.y);
		pos.z=ini.GetReal("camera", "cz", pos.z);

		target.x=ini.GetReal("camera", "tx", target.x);
		target.y=ini.GetReal("camera", "ty", target.y);
		target.z=ini.GetReal("camera", "tz", target.z);

		fow=ini.GetReal("camera", "fow", fow);

		wpos.x=ini.GetReal("wifi", "wx", wpos.x);
		wpos.y=ini.GetReal("wifi", "wy", wpos.y);
		wpos.z=ini.GetReal("wifi", "wz", wpos.z);

		power=ini.GetReal("wifi", "wpow", power);
		wifirays_num=ini.GetInteger("wifi","wnum", wifirays_num);
		wrefnum=ini.GetInteger("wifi","wrefnum",wrefnum);
		reflectfine=ini.GetReal("wifi","wrefcost", reflectfine);
		ptfine=ini.GetReal("wifi", "wptcost", ptfine);

		usegauss=ini.GetBoolean("other", "useGauss", usegauss);
		wt_mode=ini.GetInteger("other", "wt_mode", wt_mode);
		refmode=ini.GetBoolean("other", "refmode", refmode);

		findopt=ini.GetBoolean("advanced", "findopt", findopt);
		notopt=ini.GetBoolean("advanced", "nooptimize", notopt);

		grayworld=ini.GetBoolean("postprocessing", "grayworld", false);
		unsharp=ini.GetBoolean("postprocessing", "unsharp", false);
		autocontrast=ini.GetBoolean("postprocessing", "autocontrast", false);
		fraction=ini.GetReal("postprocessing", "fraction", fraction);

		if(findopt)
		{
			cout << "The search for the best place for the router is activated." << endl;
			if(notopt)
				cout<< "nooptimize=true" << endl;
		}
		else
		{
			if(wt_mode<0 || wt_mode >2)
				wt_mode=1;

			if(fow > 179)
			{
				cout << "fow must be less than 180 " <<endl;
				return 1;
			}
			cout << "imported settings:"<<endl;
			cout <<	"resolution: " << xRes<< "x" <<yRes <<endl;
			cout <<	"min wifibox resolution: " << minboxres <<endl;
			cout << "camera position: " << pos.x << " " << pos.y << " "<< pos.z << endl;
			cout << "camera target: " << target.x << " " << target.y << " "<< target.z << endl;
			cout << "fow: "<<fow <<endl;
			cout << "wifi position: " << wpos.x << " " << wpos.y << " "<< wpos.z << endl;
			cout << "wifi power: " << power  << endl;
			cout << "wifi rays number: " << wifirays_num << endl;
			cout << "max wifi rays reflection number: " << wrefnum << endl;
			cout << "wifi fog alpha: " << alpha << endl;
			cout << "cost of wifi reflecting: " << reflectfine << endl;
			cout << "cost of wifi pass through a wall: " << ptfine << endl;
			cout << "wall transparency for wifi mode: " << wt_mode << endl;
		}
	}

	//importing scene(obviously)
	cout << "importing scene..." <<endl;

	if(!importScene(f_obj.c_str()))
	{
		cout << "cannot import " << f_obj << endl;
		return 1;
	}

	cout << "scene succeffully imported " <<endl;
	double totaltime=omp_get_wtime();

	myscene.import(scene); //import from assimp structure to my structure
	myscene.tree_it();//buiding simple BVH tree


	//setting some global parameters
	maxpow=dbmtomw(power);
	weps=dbmtomw(-80);

	box_min=min_vertex(scene->mRootNode);
	box_max=max_vertex(scene->mRootNode);
	box_min-=0.1;
	box_max+=0.1;

	float h,w,l;

	h=box_max.y-box_min.y;
	w=box_max.z-box_min.z;
	l=box_max.x-box_min.x;
	if(h<w && h<l)
	{
		y_num=minboxres;
		x_num=y_num*l/h;
		z_num=y_num*w/h;
	}
	else if(w<l && w<h)
	{
		z_num=minboxres;
		x_num=z_num*l/w;
		y_num=z_num*h/w;
	}
	else
	{
		x_num=minboxres;
		y_num=x_num*h/l;
		z_num=x_num*w/l;
	}
	d = l/x_num;
	cout <<	"wifibox resolution: " << x_num << "x" << y_num << "x" << z_num <<endl;

	//initializing wifibox x,y,z

	wifibox = new float**[x_num];
	for(int i=0; i< x_num; ++i)
	{
		wifibox[i]=new float*[y_num];
		for(int j=0; j<y_num; ++j)
		{
			wifibox[i][j]=new float[z_num];
			for(int z=0; z < z_num; ++z)
				wifibox[i][j][z]=0;
		}
	}

	heatmap.createDefaultHeatMapGradient();//creating heatmap
	if(findopt)
	{
		double tim=omp_get_wtime();
		int N, iters, Res;
		bool vis, mypos;
		float minpow,power,wnum,wrefnum,step,gamma, ostanov;
		float op=0;
		vec3 pos;
		INIReader ini(f_set);//esli uj popali syda, zna4it bul vveden fail configuracii
		N=ini.GetInteger("advanced", "N", 1);
		iters=ini.GetReal("advanced", "iters", 100);
		vis=ini.GetBoolean("advanced", "visualize", false);
		minpow=ini.GetReal("advanced", "minpow", 0);
		power=ini.GetReal("advanced", "power", 20);
		wnum=ini.GetInteger("advanced", "wnum", 10000);
		wrefnum=ini.GetInteger("advanced", "wrefnum", 3);
		step=ini.GetReal("advanced", "step", 1.0f);
		gamma=ini.GetReal("advanced", "gamma", 0.7);
		ostanov=ini.GetReal("advanced", "ostanov", 0.01);
		Res=ini.GetInteger("advanced", "Res", 512);
		mypos=ini.GetBoolean("advanced", "mypos", false);
		pos.x=ini.GetReal("advanced", "xpos", 0);
		pos.y=ini.GetReal("advanced", "ypos", 0);
		pos.z=ini.GetReal("advanced", "zpos", 0);

		for(int i=0;i<N;++i)
		{
			vec3 tp;
			float top;
			tp=findoptimal(iters, minpow, power, wnum, wrefnum, step, ostanov, gamma, vis, Res, pos, mypos, top, i);
			if(top>op)
			{
				wpos=tp;
				op=top;
			}
		}


		tim=omp_get_wtime()-tim;

		cout << "elapsed time by searching optimal place: " << tim << " seconds." << endl;
	}

	//raying wifi
	double timew=omp_get_wtime();
	Wifi router = Wifi(wpos,dbmtomw(power));//initialise router
	router.start(wifirays_num, wrefnum);//raying
	timew=omp_get_wtime()-timew;
	cout << "elapsed time by wifi: " << timew << " seconds." << endl;

	//smoothing
	if(usegauss)
		Gaussfilter();
	else
		Boxfilter();

	//adding router model
	myscene.addrouter(wpos);

	double time=omp_get_wtime();
	Camera cam = Camera(pos, target,xRes,yRes, fow);//initialize camera
	std::shared_ptr<BMP> pic = cam.take();//raying
	time=omp_get_wtime()-time;

	//postrocessing
	if(grayworld)
	{
		cout << "Applying gray world" << endl;
		gray_world(pic);
	}
	if(unsharp)
	{
		cout << "Applying unsharp" << endl;
		f_unsharp(pic);
	}
	if(autocontrast)
	{
		cout << "Applying autocontrast" << endl;
		autocon(pic, fraction);
	}

	//writing result pic
	pic->WriteToFile("out.bmp");//writing to file

	cout << "elapsed time by ray: " << time << " seconds." << endl;

	//free wifibox
	for(int i=0; i< x_num; ++i)
	{
		for(int j=0; j<y_num; ++j)
			delete[] wifibox[i][j];
		delete[] wifibox[i];
	}
	delete[] wifibox;

	totaltime=omp_get_wtime()-totaltime;
	cout << "Total time: " << totaltime << " seconds." << endl;

	return 0;
}
