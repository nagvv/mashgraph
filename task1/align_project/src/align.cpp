#include "align.h"
#include <string>

#include <cmath>
#include <unordered_map>
#include <vector>


using std::string;
using std::cout;
using std::endl;


//метрики
	//Среднеквадратичное отклонение
double sko(Image img1,Image img2)
{
	double ret=0.0;
	uint h=img1.n_rows, w=img1.n_cols;
	for(uint i=0; i< h; i++)
		for(uint j=0;j<w;j++)
			ret+=std::pow(static_cast<int>(std::get<0>(img1(i,j)))-static_cast<int>(std::get<0>(img2(i,j))),2);
	return ret/(h*w);
}
	//Кросс-корреляция
double kk(Image img1, Image img2)
{
	double ret=0.0;
	uint h=img1.n_rows, w=img1.n_cols;
	for(uint i=0; i< h; i++)
		for(uint j=0;j<w;j++)
			ret+=std::get<0>(img1(i,j))*std::get<0>(img1(i,j));
	return ret;
}
	//Выбранная мною метрика + обрезка краев для лучшей точности
double best(Image img1, Image img2)
{
	uint h=img1.n_rows, w=img1.n_cols;
	Image t1=img1.submatrix(h*0.1, w*0.1, h*0.8, w*0.8);
	Image t2=img2.submatrix(h*0.1, w*0.1, h*0.8, w*0.8);
	return sko(t1,t2);
}

//Сдвиг матрицы
Image sdvig(Image img, int sh, int sw)
{
	uint h=img.n_rows, w=img.n_cols;
	Image ret(h,w);
	for(uint i=0; i< h; i++)
		for(uint j=0;j<w;j++)
			if(static_cast<int>(i)<-sh || i+sh>=h || static_cast<int>(j)<-sw || j+sw>=w)
				ret(i,j)=std::make_tuple<uint,uint,uint>(0,0,0);
			else
				ret(i,j)=img(i+sh,j+sw);
	return ret;
}

Image align(Image srcImage, bool isPostprocessing, std::string postprocessingType, double fraction, bool isMirror, 
            bool isInterp, bool isSubpixel, double subScale)
{
	if(isSubpixel)
		srcImage=resize(srcImage,subScale);


	uint height=srcImage.n_rows/3, width=srcImage.n_cols;
	//изображения по каналам
	Image imgb=srcImage.submatrix(0,0,height,width);
	Image imgg=srcImage.submatrix(height,0,height,width);
	Image imgr=srcImage.submatrix(height*2,0,height,width);

	int sh=0,sw=0;//сдвиг синего относительно зеленого
	double bt=-1;//метрика сдвига (для поиска лучшего)

	for(int th= -static_cast<int>(height)*0.05; th < height*0.05; th++)//идём по сдвигам
		for(int tw= -static_cast<int>(width)*0.05; tw < width*0.05; tw++)//идём по сдвигам
		{
			double tmp=best(imgg,sdvig(imgb, th, tw));
			if(bt<0 || tmp<bt)
			{
				bt=tmp;
				sh=th; sw=tw;
			}
		}
	imgb=sdvig(imgb,sh,sw);
	cout << "sdvig with "<< bt << ". to " << sh << " " << sw << endl;
	//Аналогично сдвиг красного относительно зелёного
	sh=0;sw=0;bt=-1;
	for(int th= -static_cast<int>(height)*0.05; th < height*0.05; th++)//идём по сдвигам
		for(int tw= -static_cast<int>(width)*0.05; tw < width*0.05; tw++)//идём по сдвигам
		{
			double tmp=best(imgg,sdvig(imgr, th, tw));
			if(bt<0 || tmp<bt)
			{
				bt=tmp;
				sh=th; sw=tw;
			}
		}
	imgr=sdvig(imgr,sh,sw);
	cout << "sdvig with "<< bt << ". to " << sh << " " << sw << endl;

	Image ret(height,width);//Итоговое изображение

	//обьединяю изображения
	for(uint i=0;i<height;i++)
		for(uint j=0;j<width;j++)
		{
			uint a=std::get<0>(imgr(i,j)),b=std::get<0>(imgg(i,j)),c=std::get<0>(imgb(i,j));
			ret(i,j)=std::make_tuple(a, b, c);
		}

	if(isSubpixel)
		ret=resize(ret,1.0/subScale);
	
	if(isPostprocessing)
	{
		if(postprocessingType=="--gray-world")
		{
			ret=gray_world(ret);
		}
		else if(postprocessingType=="--unsharp")
			ret=unsharp(ret);
		else if(postprocessingType=="--autocontrast")
			ret=autocontrast(ret,fraction);
	}
    return ret;
}

Image fix(Image ret)
{
	int r,g,b;
	for(uint i=0; i<ret.n_rows; ++i)
		for(uint j=0; j<ret.n_cols; ++j)
		{
			std::tie(r, g, b) = ret(i,j);
			r=r<0 ? 0 : (r>255 ? 255 : r);
			g=g<0 ? 0 : (g>255 ? 255 : g);
			b=b<0 ? 0 :( b>255 ? 255 : b);
			ret(i,j)=std::make_tuple(r,g,b);
		}
	return ret;
}

Image sobel_x(Image src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom(src_image, kernel);
}

Image sobel_y(Image src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom(src_image, kernel);
}

Image unsharp(Image src_image) {
	Matrix<double> kernel = {{ -1.0/6,  -2.0/3,  -1.0/6},
                             { -2.0/3,  13.0/3,  -2.0/3},
                             { -1.0/6, -2.0/3, -1.0/6}};
    return fix(custom(src_image, kernel));
}

Image gray_world(Image src_image) {
	int r,g,b;
	unsigned long s=0,sr=0,sg=0,sb=0;
	for(uint i=0; i<src_image.n_rows; ++i)
		for(uint j=0; j<src_image.n_cols; ++j)
		{
			std::tie(r, g, b) = src_image(i,j);
			sr+=r;sg+=g;sb+=b;
		}
	sr/=(src_image.n_rows*src_image.n_cols);
	sg/=(src_image.n_rows*src_image.n_cols);
	sb/=(src_image.n_rows*src_image.n_cols);
	s=(sr+sg+sb)/3;
	for(uint i=0; i<src_image.n_rows; ++i)
		for(uint j=0; j<src_image.n_cols; ++j)
		{
			std::tie(r, g, b) = src_image(i,j);
			r*=s; r/=sr;
			g*=s; g/=sg;
			b*=s; b/=sb;
			src_image(i,j)=std::make_tuple(r,g,b);
		}
    return fix(src_image);
}

Image resize(Image src_image, double scale) {
	Image ret(src_image.n_rows*scale,src_image.n_cols*scale);
	//uint r1,g1,b1 ,r2,g2,b2 ,r3,g3,b3,r4,g4,b4,r,g,b;
	for(uint i=0;i<ret.n_rows;++i)
		for(uint j=0;j<ret.n_cols;++j)
		{/*
			if(i!=ret.n_rows-1 && j!=ret.n_cols-1)
			{
				uint ti=i*src_image.n_rows/ret.n_rows;
				uint tj=j*src_image.n_cols/ret.n_cols;
				double di=i*src_image.n_rows%ret.n_rows;
				double dj=j*src_image.n_cols%ret.n_cols;
				std::tie(r1, g1, b1) = src_image(ti,tj);
				std::tie(r2, g2, b2) = src_image(ti,tj+1);
				std::tie(r3, g3, b3) = src_image(ti+1,tj);
				std::tie(r4, g4, b4) = src_image(ti+1,tj+1);
				r=static_cast<double>(r1)*(1.0-di)*(1.0-dj)+static_cast<double>(r2)*(di)*(1.0-dj)+\
						static_cast<double>(r3)*(1.0-di)*dj+static_cast<double>(r4)*di*dj;
				g=static_cast<double>(g1)*(1.0-di)*(1.0-dj)+static_cast<double>(g2)*(di)*(1.0-dj)+\
						static_cast<double>(g3)*(1.0-di)*dj+static_cast<double>(g4)*di*dj;
				b=static_cast<double>(b1)*(1.0-di)*(1.0-dj)+static_cast<double>(b2)*(di)*(1.0-dj)+\
						static_cast<double>(b3)*(1.0-di)*dj+static_cast<double>(b4)*di*dj;

			}*/
			uint ti=i*src_image.n_rows/ret.n_rows;
			uint tj=j*src_image.n_cols/ret.n_cols;
			ret(i,j) = src_image(ti,tj);
		}
    return ret;
}

class customCore
{
public:
	int radius;
	Matrix<double> kernel;

	customCore(Matrix<double> tkernel):radius((tkernel.n_cols-1)/2),kernel(tkernel){}

	std::tuple<uint, uint, uint> operator () (const Image &m) const
	{
		uint r, g, b, sum_r = 0, sum_g = 0, sum_b = 0;
		for (int i = -radius; i <= radius; ++i) {
			for (int j = -radius; j <= radius; ++j) {
				std::tie(r, g, b) = m(i+radius, j+radius);
				sum_r += r*kernel(i+radius, j+radius);
				sum_g += g*kernel(i+radius, j+radius);
				sum_b += b*kernel(i+radius, j+radius);

			}
		}
		return std::make_tuple(sum_r, sum_g, sum_b);
	}
};


Image custom(Image src_image, Matrix<double> kernel) {
    // Function custom is useful for making concrete linear filtrations
    // like gaussian or sobel. So, we assume that you implement custom
    // and then implement other filtrations using this function.
    // sobel_x and sobel_y are given as an example.
    return src_image.unary_map(customCore(kernel));
}

Image autocontrast(Image src_image, double fraction) {
	Matrix<uint> yark(src_image.n_rows, src_image.n_cols);
	std::vector<uint> y;
	for(uint i=0; i<src_image.n_rows; ++i)
		for(uint j=0; j<src_image.n_cols; ++j)
		{
			uint t=0.2125*std::get<0>(src_image(i,j))+\
					0.7154*std::get<1>(src_image(i,j))+\
					0.0721*std::get<2>(src_image(i,j));
			y.push_back(t);
		}
	std::sort(y.begin(),y.end());
	uint N=src_image.n_rows*src_image.n_cols;
	int min=y[N*fraction], max=y[y.size()-1-N*fraction];
	int r,g,b;
	for(uint i=0; i<src_image.n_rows; ++i)
		for(uint j=0; j<src_image.n_cols; ++j)
		{
			std::tie(r, g, b) = src_image(i,j);
			r=r<min ? 0 : r>max ? 255 : (r-min)*(255)/(max-min);
			g=g<min ? 0 : g>max ? 255 : (g-min)*(255)/(max-min);
			b=b<min ? 0 : b>max ? 255 : (b-min)*(255)/(max-min);
			//r=r<0 ? 0 : (r>255 ? 255 : r);
			//g=g<0 ? 0 : (g>255 ? 255 : g);
			//b=b<0 ? 0 :( b>255 ? 255 : b);
			src_image(i,j)=std::make_tuple(r,g,b);
		}

    return (src_image);
}


Image gaussian(Image src_image, double sigma, int radius)  {
	Matrix<double> kernel(2*radius+1,2*radius+1);
	double pi=3.1415926535;
	double sum=0;
	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
		{
			kernel(i+radius,j+radius)=1/(2*pi*std::pow(sigma,2))*std::exp(-(i*i+j*j)/(2*std::pow(sigma,2)));
			sum+=kernel(i+radius,j+radius);
		}
	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
			kernel(i+radius,j+radius)/=sum;
    return fix(custom(src_image, kernel));
}

Image gaussian_separable(Image src_image, double sigma, int radius) {
	Matrix<double> kernelx(2*radius+1,2*radius+1), kernely(2*radius+1,2*radius+1);
	double pi=3.1415926535;
	double sum=0;

	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
			if(i!=0)
				kernelx(i+radius,j+radius) = 0;
			else
			{
				kernelx(i+radius,j+radius)=1/(2*pi*std::pow(sigma,2))*std::exp(-(j*j)/(2*std::pow(sigma,2)));
				sum+=kernelx(i+radius,j+radius);
			}

	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
			if(i==0)
				kernelx(i+radius,j+radius)/=sum;
	sum=0;
	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
			if(j!=0)
				kernely(i+radius,j+radius) = 0;
			else
			{
				kernely(i+radius,j+radius)=1/(2*pi*std::pow(sigma,2))*std::exp(-(i*i)/(2*std::pow(sigma,2)));
				sum+=kernely(i+radius,j+radius);
			}
	for (int i = -radius; i <= radius; ++i)
		for (int j = -radius; j <= radius; ++j)
			if(j==0)
				kernely(i+radius,j+radius)/=sum;

    return fix(custom(custom(src_image, kernelx),kernely));
}

Image median(Image src_image, int radius) {
	Image med(src_image.n_rows,src_image.n_cols);
	for(int i=0; i<static_cast<int>(src_image.n_rows); ++i)
		for(int j=0; j<static_cast<int>(src_image.n_cols); ++j)
		{
			if(i<radius || j<radius || i>static_cast<int>(src_image.n_rows)-1-radius || j>static_cast<int>(src_image.n_cols)-1-radius)
				med(i,j)=src_image(i,j);
			else
			{
				std::vector<uint> r,g,b;
				for (int t = -radius; t <= radius; ++t)
					for (int y = -radius; y <= radius; ++y)
					{
						r.push_back(std::get<0>(src_image(i+t,j+y)));
						g.push_back(std::get<1>(src_image(i+t,j+y)));
						b.push_back(std::get<2>(src_image(i+t,j+y)));
					}
				std::sort(r.begin(),r.end());
				std::sort(g.begin(),g.end());
				std::sort(b.begin(),b.end());
				med(i,j)=std::make_tuple(r[r.size()/2], g[g.size()/2], b[b.size()/2]);
			}
		}
    return med;
}

Image median_linear(Image src_image, int radius) {
    return src_image;
}

Image median_const(Image src_image, int radius) {
    return src_image;
}

Image canny(Image src_image, int threshold1, int threshold2) {
	//1 сглаживание
	Image img = gaussian(src_image, 1.4, 2);
	Image ix=sobel_x(img), iy=sobel_y(img);//Почему оператор собеля записываает свой результат(которая бывает отрицательным) в Image, который unsigned int.
	//2 градиент
	Matrix<uint> grad(img.n_rows,img.n_cols);
	Matrix<uint> pgrad(img.n_rows,img.n_cols);
	for(uint i=0;i<img.n_rows;++i)
		for(uint j=0;j<img.n_cols;++j)
		{
			//(ix^2+iy^2)^0.5
			pgrad(i,j)=std::pow(std::pow(static_cast<int>(std::get<0>(ix(i,j))),2)+std::pow(static_cast<int>(std::get<0>(iy(i,j))),2),0.5);
			//std::get<1>(grad(i,j))=std::pow(std::pow(std::get<1>(ix(i,j)),2)+std::pow(std::get<1>(iy(i,j)),2),0.5);
			//std::get<2>(grad(i,j))=std::pow(std::pow(std::get<2>(ix(i,j)),2)+std::pow(std::get<2>(iy(i,j)),2),0.5);
		}
	Matrix<double> napr(img.n_rows,img.n_cols);
	for(uint i=0;i<img.n_rows;++i)
		for(uint j=0;j<img.n_cols;++j)
		{
			//atan2(iy,ix)
			napr(i,j)=std::atan2(std::get<0>(iy(i,j)),std::get<0>(ix(i,j)));
		}

	//3 подавление немакс
	double pi=3.1415926535;
	for(uint i=1;i<img.n_rows-1;++i)
		for(uint j=1;j<img.n_cols-1;++j)
		{
			grad(i,j)=pgrad(i,j);
			double t= napr(i,j)+pi;//t in [0, 2pi]
			t = t*4/pi; //t in [0, 8]
			int nap = static_cast<int>(t+0.5);//в итого направо это 4, вниз -2, влево 0 и 8
			switch(nap)
			{
			case 8:
			case 0:
			case 4:
				if(pgrad(i,j)<pgrad(i,j-1) || pgrad(i,j)<pgrad(i,j+1))
					grad(i,j)=0;
				break;
			case 1:
			case 5:
				if(pgrad(i,j)<pgrad(i+1,j-1) || pgrad(i,j)<pgrad(i-1,j+1))
					grad(i,j)=0;
				break;
			case 2:
			case 6:
				if(pgrad(i,j)<pgrad(i+1,j) || pgrad(i,j)<pgrad(i-1,j))
					grad(i,j)=0;
				break;
			case 3:
			case 7:
				if(pgrad(i,j)<pgrad(i+1,j+1) || pgrad(i,j)<pgrad(i-1,j-1))
					grad(i,j)=0;
				break;
			default:
				if(pgrad(i,j)<pgrad(i,j-1) || pgrad(i,j)<pgrad(i,j+1))
					grad(i,j)=0;
				break;
			}
			if(pgrad(i,j)<static_cast<uint>(threshold1))//<-- обнуление по первому порогу
				grad(i,j)=0;//          |
		}//                             |
	//4 отсечение по двум порогам  ------
	//5 остлеживание границ по гистерезицу

	std::vector<std::vector<std::pair<uint,uint>>> gr;
	std::unordered_map<uint, uint> pw; //1-weak, 2-stronk!
	for(uint i=1;i<img.n_rows-1;++i)
		for(uint j=1;j<img.n_cols-1;++j)
		{
			if(grad(i,j)>=static_cast<uint>(threshold1))
			{
				auto place = gr.end();//в какой куче лежит этот пиксель(если лежит и далее найдется)
				if(grad(i-1,j-1)>=static_cast<uint>(threshold1))//смотрим верхний левый
				{
					bool b=false;//этот пиксел в куче
					auto tit=gr.end();//если в куче то в этой
					for(auto it=gr.begin(); it!=gr.end(); it++)//ищем его
					{
						if(b)
							break;
						for(auto t=(*it).begin(); t!=(*it).end(); t++)
							if((*t)==std::make_pair(i-1,j-1))//нашли
							{
								b=true;
								tit=it;
								break;
							}
					}
					if(b)
					{
						(*tit).push_back(std::make_pair(i,j));//добавляю к его куче
						place=tit;
					}
				}
				if(grad(i-1,j)>=static_cast<uint>(threshold1))//ищем\обьединяем кучи
				{
					bool b=false;//этот пиксел в куче
					auto tit=gr.end();//если в куче то в этой
					for(auto it=gr.begin(); it!=gr.end(); it++)//ищем его
					{
						if(b)
							break;
						for(auto t=(*it).begin(); t!=(*it).end(); t++)
							if((*t)==std::make_pair(i-1,j))//нашли
							{
								b=true;
								tit=it;
								break;
							}
					}
					if(b)
					{
						if(place==gr.end())//добавляю в кучу
						{
							(*tit).push_back(std::make_pair(i,j));//добавляю к его куче
							place=tit;
						}
						else if(place!=tit)//или обьединяю кучи
						{
							for(auto t=(*tit).begin(); t!=(*tit).end();t++)
								(*place).push_back((*t));
							gr.erase(tit);
						}
					}
				}
				if(grad(i-1,j+1)>=static_cast<uint>(threshold1))//ищем\обьединяем кучи
				{
					bool b=false;//этот пиксел в куче
					auto tit=gr.end();//если в куче то в этой
					for(auto it=gr.begin(); it!=gr.end(); it++)//ищем его
					{
						if(b)
							break;
						for(auto t=(*it).begin(); t!=(*it).end(); t++)
							if((*t)==std::make_pair(i-1,j+1))//нашли
							{
								b=true;
								tit=it;
								break;
							}
					}
					if(b)
					{
						if(place==gr.end())//добавляю в кучу
						{
							(*tit).push_back(std::make_pair(i,j));//добавляю к его куче
							place=tit;
						}
						else if(place!=tit)//или обьединяю кучи
						{
							for(auto t=(*tit).begin(); t!=(*tit).end();t++)
								(*place).push_back((*t));
							gr.erase(tit);
						}
					}
				}
				if(grad(i,j-1)>=static_cast<uint>(threshold1))//ищем\обьединяем кучи
				{
					bool b=false;//этот пиксел в куче
					auto tit=gr.end();//если в куче то в этой
					for(auto it=gr.begin(); it!=gr.end(); it++)//ищем его
					{
						if(b)
							break;
						for(auto t=(*it).begin(); t!=(*it).end(); t++)
							if((*t)==std::make_pair(i,j-1))//нашли
							{
								b=true;
								tit=it;
								break;
							}
					}
					if(b)
					{
						if(place==gr.end())//добавляю в кучу
						{
							(*tit).push_back(std::make_pair(i,j));//добавляю к его куче
							place=tit;
						}
						else if(place!=tit)//или обьединяю кучи
						{
							for(auto t=(*tit).begin(); t!=(*tit).end();t++)
								(*place).push_back((*t));
							gr.erase(tit);
						}
					}
				}
				if(place==gr.end())//если не были найдены кучи по соседству спереди
				{
					gr.emplace_back();
					gr.back().push_back(std::make_pair(i,j));
				}
			}
		}


	Matrix<uint> gran(img.n_rows, img.n_cols);//карта границ

	for(auto it=gr.begin(); it!=gr.end(); it++)
	{
		if((*it).size()<=1)//одинокий несоединенный пиксель/пропускаем
			continue;
		bool b=false;//есть сильный градиент ---> попадает на карту
		for(auto t=(*it).begin(); t!=(*it).end(); t++)
		{
			if( grad( (*t).first, (*t).second ) > static_cast<uint>(threshold2) )
			{
				b=true;
				break;
			}
		}
		if(b)
			for(auto t=(*it).begin(); t!=(*it).end(); t++)
				gran( (*t).first, (*t).second ) = 1;

	}
	//обрезаем границы
	uint o1,o2,o3,o4; //границы обрезки сверху, справа, снизу, слева
	uint m1=0,m2=0, i1=0,i2=0;
	uint okr1=4, okr2=3;

	//ищем верхнюю границу
	for(uint i=okr1; i < gran.n_rows*0.03; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_cols; j++)
			n+=gran(i,j);
		if(n>m1)
		{
			m1=n;
			i1=i;
		}
	}
	for(uint i=okr1; i < gran.n_rows*0.03; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_cols; j++)
			n+=gran(i,j);
		if(n>m2 && (i1>i ? i1-i : i-i1)>okr2)//окрестность в okr2 пикселя
		{
			m2=n;
			i2=i;
		}
	}
	o1=i1>i2?i1:i2;
	//аналогично с другими границами
	m1=0;m2=0;i1=0;i2=0;
	for(uint i=okr1; i < gran.n_cols*0.08; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_rows; j++)
			n+=gran(j,gran.n_cols-1-i);
		if(n>m1)
		{
			m1=n;
			i1=i;
		}
	}
	for(uint i=okr1; i < gran.n_cols*0.08; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_rows; j++)
			n+=gran(j,gran.n_cols-1-i);
		if(n>m2 && (i1>i ? i1-i : i-i1)>okr2)//окрестность в okr2 пикселя
		{
			m2=n;
			i2=i;
		}
	}
	o2=i1>i2?i1:i2;
	m1=0;m2=0;i1=0;i2=0;
	for(uint i=okr1; i < gran.n_rows*0.03; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_cols; j++)
			n+=gran(gran.n_rows-1-i,j);
		if(n>m1)
		{
			m1=n;
			i1=i;
		}
	}
	for(uint i=okr1; i < gran.n_rows*0.03; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_cols; j++)
			n+=gran(gran.n_rows-1-i,j);
		if(n>m2 && (i1>i ? i1-i : i-i1)>okr2)//окрестность в okr2 пикселя
		{
			m2=n;
			i2=i;
		}
	}
	o3=i1>i2?i1:i2;
	m1=0;m2=0;i1=0;i2=0;
	for(uint i=okr1; i < gran.n_cols*0.08; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_rows; j++)
			n+=gran(j,i);
		if(n>m1)
		{
			m1=n;
			i1=i;
		}
	}
	for(uint i=okr1; i < gran.n_cols*0.08; i++)
	{
		uint n=0;
		for(uint j=0; j < gran.n_rows; j++)
			n+=gran(j,i);
		if(n>m2 && (i1>i ? i1-i : i-i1)>okr2)//окрестность в okr2 пикселя
		{
			m2=n;
			i2=i;
		}
	}
	o4=i1>i2?i1:i2;
	return src_image.submatrix(o1,o4,src_image.n_rows-o1-o3,src_image.n_cols-o2-o4);
}



















