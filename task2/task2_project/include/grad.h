#pragma once
#include "matrix.h"

template <typename T>
class customCore
{
public:
	int radius;
	int &vert_radius=radius;
	int &hor_radius=radius;
	Matrix<double> kernel;

	customCore(Matrix<double> tkernel):radius((tkernel.n_cols-1)/2),kernel(tkernel){}

	T operator () (const Matrix<T> &m) const
	{
		T sum = 0;
		for (int i = -radius; i <= radius; ++i) {
			for (int j = -radius; j <= radius; ++j) {
				sum += m(i+radius, j+radius)*kernel(i+radius, j+radius);
			}
		}
		return sum;
	}
};

template <typename T>
Matrix<T> custom(Matrix<T> src_image, Matrix<double> kernel) {
    return src_image.unary_map(customCore<T>(kernel));
}

template <typename T>
Matrix<T> sobel_x(Matrix<T> src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom<T>(src_image, kernel);
}

template <typename T>
Matrix<T> sobel_y(Matrix<T> src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom<T>(src_image, kernel);
}

template <typename T>
Matrix<T> resize(Matrix<T> src_image, double scale) {
	Matrix<T> ret(src_image.n_rows*scale,src_image.n_cols*scale);
	for(int i=0;i<ret.n_rows;++i)
		for(int j=0;j<ret.n_cols;++j)
		{
			int ti=i*src_image.n_rows/ret.n_rows;
			int tj=j*src_image.n_cols/ret.n_cols;
			ret(i,j) = src_image(ti,tj);
		}
    return ret;
}

template <typename T>
Matrix<T> resize(Matrix<T> src_image, int h, int w)
{
	Matrix<T> ret(h,w);
	for(int i=0;i<h;++i)
		for(int j=0;j<w;++j)
		{
			int ti=i*src_image.n_rows/h;
			int tj=j*src_image.n_cols/w;
			ret(i,j) = src_image(ti,tj);
		}
    return ret;
}
