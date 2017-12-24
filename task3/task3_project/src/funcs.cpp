#include "funcs.h"
#include "Wifi.h"
#include "Camera.h"
#include "gifh/gif.h"

using glm::vec3;


float dbmtomw(float power)
{
	return pow(10.0f, power/10);
}

float mwtodbm(float lpower)
{
	return 10*std::log10(lpower);
}

vec3 max_vertex(aiNode *node)
{
	vec3 ret = vec3(0,0,0);
	int childs = node->mNumChildren;
	int meshes = node->mNumMeshes;

	for(int i=0; i<childs; ++i)
	{
		vec3 temp;
		temp=max_vertex(node->mChildren[i]);
		if( temp.x > ret.x )
			ret.x=temp.x;
		if( temp.y > ret.y )
			ret.y=temp.y;
		if( temp.z > ret.z )
			ret.z=temp.z;
	}

	for(int i=0; i<meshes; ++i)
	{
		int index= node->mMeshes[i];
		int verts = scene->mMeshes[index]->mNumVertices;
		for(int j=0; j< verts; ++j)
		{
			vec3 temp;
			temp.x=scene->mMeshes[index]->mVertices[j].x;
			temp.y=scene->mMeshes[index]->mVertices[j].y;
			temp.z=scene->mMeshes[index]->mVertices[j].z;
			if( temp.x > ret.x )
				ret.x=temp.x;
			if( temp.y > ret.y )
				ret.y=temp.y;
			if( temp.z > ret.z )
				ret.z=temp.z;
		}
	}
	return ret;
}

vec3 min_vertex(aiNode *node)
{
	vec3 ret = vec3(0,0,0);
	int childs = node->mNumChildren;
	int meshes = node->mNumMeshes;

	for(int i=0; i<childs; ++i)
	{
		vec3 temp;
		temp=min_vertex(node->mChildren[i]);
		if( temp.x < ret.x )
			ret.x=temp.x;
		if( temp.y < ret.y )
			ret.y=temp.y;
		if( temp.z < ret.z )
			ret.z=temp.z;
	}

	for(int i=0; i<meshes; ++i)
	{
		int index= node->mMeshes[i];
		int verts = scene->mMeshes[index]->mNumVertices;
		for(int j=0; j< verts; ++j)
		{
			vec3 temp;
			temp.x=scene->mMeshes[index]->mVertices[j].x;
			temp.y=scene->mMeshes[index]->mVertices[j].y;
			temp.z=scene->mMeshes[index]->mVertices[j].z;
			if( temp.x < ret.x )
				ret.x=temp.x;
			if( temp.y < ret.y )
				ret.y=temp.y;
			if( temp.z < ret.z )
				ret.z=temp.z;
		}
	}
	return ret;
}



void Boxfilter()
{
	int N=5;
	float*** kernel;
	kernel = new float**[N];
	for(int i=0; i< N; ++i)
	{
		kernel[i]=new float*[N];
		for(int j=0; j<N; ++j)
		{
			kernel[i][j]=new float[N];
			for(int z=0; z < N; ++z)
				kernel[i][j][z]=1.0f/(N*N*N);
		}
	}


	float*** blured;
	blured = new float**[x_num];
	for(int i=0; i< x_num; ++i)
	{
		blured[i]=new float*[y_num];
		for(int j=0; j<y_num; ++j)
		{
			blured[i][j]=new float[z_num];
			for(int z=0; z < z_num; ++z)
				blured[i][j][z]=0;
		}
	}

	int R=N/2;
	#pragma omp parallel for shared(blured)
	for(int x=0; x<x_num; ++x)
		for(int y=0; y<y_num; ++y)
			for(int z=0; z<z_num; ++z)
			{
				for(int i=-R; i<=R; ++i)
					for(int j=-R; j<=R; ++j)
						for(int k=-R; k<=R; ++k)
						{
							if(x+i>=0 && x+i <x_num && y+j>=0 && y+j <y_num && z+k>=0 && z+k<z_num)
								blured[x][y][z]+=wifibox[x+i][y+j][z+k]*kernel[i+R][j+R][k+R];
						}
			}


	float*** temp;
	temp=wifibox;
	wifibox=blured;
	for(int i=0; i< x_num; ++i)
	{
		for(int j=0; j<y_num; ++j)
			delete[] temp[i][j];
		delete[] temp[i];
	}
	delete[] temp;
}

void Gaussfilter()
{
	int N=5;
	float sigma=1;
	float pi=3.14159265359;
	float*** kernel;
	int R=N/2;
	kernel = new float**[N];
	float sum=0;
	for(int i=-R; i<= R; ++i)
	{
		kernel[i+R]=new float*[N];
		for(int j=-R; j<=R; ++j)
		{
			kernel[i+R][j+R]=new float[N];
			for(int z=-R; z <=R; ++z)
			{
				kernel[i+R][j+R][z+R]=1.0f/(std::pow(2*pi*sigma*sigma, 1.5))*std::exp(-(i*i+j*j+z*z)/(2*sigma*sigma));
				sum+=kernel[i+R][j+R][z+R];
			}
		}
	}


	//normalize
	for(int i=0; i< N; ++i)
	{
		for(int j=0; j<N; ++j)
		{
			for(int z=0; z < N; ++z)
				kernel[i][j][z]/=sum;
		}
	}




	float*** blured;
	blured = new float**[x_num];
	for(int i=0; i< x_num; ++i)
	{
		blured[i]=new float*[y_num];
		for(int j=0; j<y_num; ++j)
		{
			blured[i][j]=new float[z_num];
			for(int z=0; z < z_num; ++z)
				blured[i][j][z]=0;
		}
	}


	#pragma omp parallel for shared(blured)
	for(int x=0; x<x_num; ++x)
		for(int y=0; y<y_num; ++y)
			for(int z=0; z<z_num; ++z)
			{
				for(int i=-R; i<=R; ++i)
					for(int j=-R; j<=R; ++j)
						for(int k=-R; k<=R; ++k)
						{
							if(x+i>=0 && x+i <x_num && y+j>=0 && y+j <y_num && z+k>=0 && z+k<z_num)
								blured[x][y][z]+=wifibox[x+i][y+j][z+k]*kernel[i+R][j+R][k+R];
						}
			}


	float*** temp;
	temp=wifibox;
	wifibox=blured;
	for(int i=0; i< x_num; ++i)
	{
		for(int j=0; j<y_num; ++j)
			delete[] temp[i][j];
		delete[] temp[i];
	}
	delete[] temp;
}

float getval(float minpower)
{
	float ret;
#pragma omp parallel for shared(wifibox, ret)
	for(int x=0; x<x_num; ++x)
		for(int y=0; y<y_num; ++y)
			for(int z=0; z<z_num; ++z)
				if(wifibox[x][y][z]>minpower)
					ret++;
	return ret;
}

void clearbox()
{
#pragma omp parallel for shared(wifibox)
	for(int x=0; x<x_num; ++x)
		for(int y=0; y<y_num; ++y)
			for(int z=0; z<z_num; ++z)
				wifibox[x][y][z]=0;
}

extern vec3 box_min,box_max;

vec3 findoptimal(int iters, float minpow, float power, int wnum, int wrefnum, float step, float ostanov, float gamma, bool visual, int Res, vec3 pos, bool mypos, float &op, int num)
{
	vec3 opt(0,0,0);
	float optv=0;
	vec3 tx,ty,tz ;
	float minpower=dbmtomw(minpow);//in mW
	vec3 max=box_max, min=box_min;
	max-=0.1;min+=0.1;//obratno sujay(ranee bulo rashireno)
	if(mypos)
		opt=pos;
	else
	{
		std::srand(static_cast<unsigned>(std::time(0)));
		std::rand();//sho za xrenya. bez etogo koordinata x ne beretsya randomnum obrazom. Da i voobshe random kak-to posredstvenno rabotaet.
	//	opt=vec3(std::rand()*((max.x-min.x)/RAND_MAX), std::rand()*((max.y-min.y)/RAND_MAX), std::rand()*((max.z-min.z)/RAND_MAX));
	//	opt+=min;

		opt.x=min.x + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX/(max.x - min.x));
		opt.y=min.y + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX/(max.y - min.y));
		opt.z=min.z + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX/(max.z - min.z));
	}
	Wifi router = Wifi(opt,dbmtomw(power));//initialise router
	router.start(wnum, wrefnum);//raying
	Gaussfilter();
	optv=getval(minpower);


	//БОЛЬШЕ ГИФОК БОГУ ГИФоК
	GifWriter gw;
	uint8_t *img ;//rgba8
	if(visual)
	{
		string str="opt/iter_"+std::to_string(num)+".gif";
		GifBegin(&gw,str.c_str(), Res, Res, 50);
		img = new uint8_t[Res*Res*4];

		vec3 pos(opt.x,opt.y+10,opt.z);
		vec3 target=opt;
		float fow=100;
		Camera cam = Camera(pos, target,Res,Res, fow);//initialize camera
		std::shared_ptr<BMP> pic = cam.take();//raying
		for(int i=0; i<Res; i++)
			for(int j=0; j<Res*4; j+=4)
			{
				img[i*Res*4+j]=pic->GetPixel(j/4,i).Red;
				img[i*Res*4+j+1]=pic->GetPixel(j/4,i).Green;
				img[i*Res*4+j+2]=pic->GetPixel(j/4,i).Blue;
				img[i*Res*4+j+3]=pic->GetPixel(j/4,i).Alpha;//ignoret by library
			}
		GifWriteFrame(&gw,img,Res,Res,50);

	}
	clearbox();

	std::cout << "Progress: ";
	for(int iter=0; iter<iters; ++iter)
	{
		//x
		float tv;
		bool moved=false;

		bool bx=true,by=true,bz=true;
		tx=vec3(opt.x+step,opt.y,opt.z);
		if(tx.x < max.x && tx.x > min.x)
		{
			Wifi router = Wifi(tx,dbmtomw(power));//initialise router
			router.start(wnum, wrefnum);//raying
			Gaussfilter();
			tv=getval(minpower);
			clearbox();
			if(tv>optv)
			{
				opt=tx;
				optv=tv;
				bx=false;
				moved=true;
			}
		}

		if(bx)
		{
			tx=vec3(opt.x-step,opt.y,opt.z);
			if(tx.x < max.x && tx.x > min.x)
			{
				Wifi router = Wifi(tx,dbmtomw(power));//initialise router
				router.start(wnum, wrefnum);//raying
				Gaussfilter();
				tv=getval(minpower);
				clearbox();
				if(tv>optv)
				{
					opt=tx;
					optv=tv;
					moved=true;
				}
			}
		}
		//y
		ty=vec3(opt.x,opt.y+step,opt.z);
		if(ty.y < max.y && ty.y > min.y)
		{
			Wifi router = Wifi(ty,dbmtomw(power));//initialise router
			router.start(wnum, wrefnum);//raying
			Gaussfilter();
			tv=getval(minpower);
			clearbox();
			if(tv>optv)
			{
				opt=ty;
				optv=tv;
				by=false;
				moved=true;
			}
		}

		if(by)
		{
			ty=vec3(opt.x,opt.y-step,opt.z);
			if(ty.y < max.y && ty.y > min.y)
			{
				Wifi router = Wifi(ty,dbmtomw(power));//initialise router
				router.start(wnum, wrefnum);//raying
				Gaussfilter();
				tv=getval(minpower);
				clearbox();
				if(tv>optv)
				{
					opt=ty;
					optv=tv;
					moved=true;
				}
			}
		}
		//z
		tz=vec3(opt.x,opt.y,opt.z+step);
		if(tz.z < max.z && tz.z > min.z)
		{
			Wifi router = Wifi(tz,dbmtomw(power));//initialise router
			router.start(wnum, wrefnum);//raying
			Gaussfilter();
			tv=getval(minpower);

			if(tv>optv)
			{
				opt=tz;
				optv=tv;
				bz=false;
				moved=true;
			}
		}

		if(bz)
		{
			tz=vec3(opt.x,opt.y,opt.z-step);
			if(tz.z < max.z && tz.z > min.z)
			{
				clearbox();
				Wifi router = Wifi(tz,dbmtomw(power));//initialise router
				router.start(wnum, wrefnum);//raying
				Gaussfilter();
				tv=getval(minpower);
				if(tv>optv)
				{
					opt=tz;
					optv=tv;
					moved=true;
				}
			}
		}
		if(!moved)
			step*=gamma;
		else
		{
			if(visual)
			{
				vec3 pos(opt.x,opt.y+10,opt.z);
				vec3 target=opt;
				float fow=100;
				Camera cam = Camera(pos, target,Res,Res, fow);//initialize camera
				std::shared_ptr<BMP> pic = cam.take();//raying
				//string str="opt/iter_"+std::to_string(iter)+".bmp";
				//pic->WriteToFile(str.c_str());//writing to file
				for(int i=0; i<Res; i++)
					for(int j=0; j<Res*4; j+=4)
					{
						img[i*Res*4+j]=pic->GetPixel(j/4,i).Red;
						img[i*Res*4+j+1]=pic->GetPixel(j/4,i).Green;
						img[i*Res*4+j+2]=pic->GetPixel(j/4,i).Blue;
						img[i*Res*4+j+3]=pic->GetPixel(j/4,i).Alpha;//ignoret by library
					}
				GifWriteFrame(&gw,img,Res,Res,50);

			}
		}
		clearbox();
		if(step<ostanov)
			break;
		std::cout <<".";

	}
	if(visual)
	{
		GifEnd(&gw);
		delete[] img;
	}
	std::cout<<std::endl;
	op=optv;
	return opt;
}


void gray_world(std::shared_ptr<BMP> img)
{
	int r,g,b;
	unsigned long s=0,sr=0,sg=0,sb=0;

	int rows=img->TellHeight();
	int cols=img->TellWidth();

	for(int i=0; i<rows; ++i)
		for(int j=0; j<cols; ++j)
		{
			r=img->GetPixel(j,i).Red;
			g=img->GetPixel(j,i).Green;
			b=img->GetPixel(j,i).Blue;
			sr+=r;sg+=g;sb+=b;
		}
	sr/=(rows*cols);
	sg/=(rows*cols);
	sb/=(rows*cols);
	s=(sr+sg+sb)/3;
	for(int i=0; i<rows; ++i)
		for(int j=0; j<cols; ++j)
		{
			r=img->GetPixel(j,i).Red;
			g=img->GetPixel(j,i).Green;
			b=img->GetPixel(j,i).Blue;

			r*=s; r/=sr;
			g*=s; g/=sg;
			b*=s; b/=sb;

			r=r<0 ? 0 : (r>255 ? 255 : r);
			g=g<0 ? 0 : (g>255 ? 255 : g);
			b=b<0 ? 0 :( b>255 ? 255 : b);

			RGBApixel pixel;
			pixel.Red=r;
			pixel.Green=g;
			pixel.Blue=b;
			img->SetPixel(j,i, pixel);
		}
}

void f_unsharp(std::shared_ptr<BMP> &img)
{
	float kernel[3][3] = {{ -1.0f/6,  -2.0f/3,  -1.0f/6},
	                             { -2.0f/3,  13.0f/3,  -2.0f/3},
	                             { -1.0f/6, -2.0f/3, -1.0f/6}};

	int radius=1;//radius
	int width=img->TellWidth();
	int height=img->TellHeight();


	shared_ptr<BMP> nimg = make_shared<BMP>();
	nimg->SetSize(width, height);

	#pragma omp parallel for shared(nimg)
	for(int x=0; x<height; ++x)
		for(int y=0; y<width; ++y)
		{
			RGBApixel pixel;
			int sr=0,sg=0,sb=0;
			int r, g, b;//rgb
			for(int i=-radius; i<=radius; ++i)
				for(int j=-radius; j<=radius; ++j)
				{
					if(x+i>=0 && x+i <height && y+j>=0 && y+j <width)
					{
						r=img->GetPixel(j+y,i+x).Red;
						g=img->GetPixel(j+y,i+x).Green;
						b=img->GetPixel(j+y,i+x).Blue;

						sr+=r*kernel[i+radius][j+radius];
						sg+=g*kernel[i+radius][j+radius];
						sb+=b*kernel[i+radius][j+radius];
					}
				}
			pixel.Red=sr<0 ? 0 : (sr>255 ? 255 : sr);
			pixel.Green=sg<0 ? 0 : (sg>255 ? 255 : sg);
			pixel.Blue=sb<0 ? 0 :( sb>255 ? 255 : sb);
//			std::cout<< "1" << pixel.Red << " " << pixel.Green << " " << pixel.Blue << std::endl;
//			std::cout<< "2" <<pixel.Red << " " << pixel.Green << " " << pixel.Blue << std::endl;
			nimg->SetPixel(y,x,pixel);
		}

	img=nimg;
}

void autocon(std::shared_ptr<BMP> img, float fraction)
{
	unsigned int width=img->TellWidth();
	unsigned int height=img->TellHeight();

	std::vector<unsigned int> y;

	for(unsigned int i=0; i<height; ++i)
		for(unsigned int j=0; j<width; ++j)
		{
			unsigned int t=0.2125*img->GetPixel(j,i).Red + 0.7154*img->GetPixel(j,i).Green + 0.0721*img->GetPixel(j,i).Blue;
			y.push_back(t);
		}
	std::sort(y.begin(),y.end());

	unsigned int N=width*height;
	int min=y[N*fraction], max=y[y.size()-1-N*fraction];
	int r,g,b;
	for(unsigned int i=0; i<height; ++i)
		for(unsigned int j=0; j<width; ++j)
		{
			r=img->GetPixel(j,i).Red;
			g=img->GetPixel(j,i).Green;
			b=img->GetPixel(j,i).Blue;

			r=r<min ? 0 : r>max ? 255 : (r-min)*(255)/(max-min);
			g=g<min ? 0 : g>max ? 255 : (g-min)*(255)/(max-min);
			b=b<min ? 0 : b>max ? 255 : (b-min)*(255)/(max-min);

			RGBApixel pixel;
			pixel.Red=r;
			pixel.Green=g;
			pixel.Blue=b;
			img->SetPixel(j,i, pixel);
		}
}


/*
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
*/
//somewhat changed by me (Nikolashkin, eee)

#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

bool HitBoundingBox(glm::vec3 minB,glm:: vec3 maxB, glm::vec3 origin, glm::vec3 dir, glm::vec3 &coord)
{
	bool inside = true;
	char quadrant[NUMDIM];
	register int i;
	int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[NUMDIM];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<NUMDIM; i++)
		if(origin[i] < minB[i])
		{
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}
		else if (origin[i] > maxB[i])
		{
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}
		else
		{
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		coord = origin;
		return true;
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
		if (quadrant[i] != MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (false);
	for (i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] *dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (false);
		} else {
			coord[i] = candidatePlane[i];
		}
	return true;				/* ray hits box */
}


/*
ColorGradient
by Andrew Noske
from www.andrewnoske.com
 */
//somewhat changed by me, yup

ColorGradient::ColorGradient() {  createDefaultHeatMapGradient();  }

void ColorGradient::addColorPoint(float red, float green, float blue, float value)
{
	for(unsigned int i=0; i<color.size(); i++)  {
	  if(value < color[i].val) {
		color.insert(color.begin()+i, ColorPoint(red,green,blue, value));
		return;  }}
	color.push_back(ColorPoint(red,green,blue, value));
}

void ColorGradient::clearGradient() { color.clear(); }

void ColorGradient::createDefaultHeatMapGradient()
{
  color.clear();
  color.push_back(ColorPoint(0, 0, 0,   0.0f));		 // black.
  color.push_back(ColorPoint(0, 0, 1,   0.17f));		 // Blue.
  color.push_back(ColorPoint(0, 1, 1,   0.34f));		//cyan
  color.push_back(ColorPoint(0, 1, 0,   0.51f));		 // Green.
  color.push_back(ColorPoint(1, 1, 0,   0.68f));	//yellow
  color.push_back(ColorPoint(1, 0, 0,   0.85f));      // Red.
  color.push_back(ColorPoint(1, 1, 1,   1.0f));      // White.
}

void ColorGradient::getColorAtValue(const float value, float &red, float &green, float &blue)
{
  if(color.size()==0)
    return;

  for(int i=0; i<static_cast<int>(color.size()); i++)
  {
    ColorPoint &currC = color[i];
    if(value < currC.val)
    {
      ColorPoint &prevC  = color[ max(0,i-1) ];
      float valueDiff    = (prevC.val - currC.val);
      float fractBetween = (valueDiff==0) ? 0 : (value - currC.val) / valueDiff;
      red   = (prevC.r - currC.r)*fractBetween + currC.r;
      green = (prevC.g - currC.g)*fractBetween + currC.g;
      blue  = (prevC.b - currC.b)*fractBetween + currC.b;
      return;
    }
  }
  red   = color.back().r;
  green = color.back().g;
  blue  = color.back().b;
  return;
}

