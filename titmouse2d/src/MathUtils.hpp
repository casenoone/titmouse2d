#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <cmath>


//计算平方
template<class T>
inline T sqr(const T& x)
{
	return x * x;
}


template<class T>
inline T clamp(const T& factor, const T& min, const T& max) {
	if (factor > max) {
		return max;
	}
	if (factor < min) {
		return min;
	}
	return factor;
}


//计算插值的权重
inline void getBarycentric(
	double x,
	int iLow,
	int iHigh,
	int* i,
	double* f) {


	double s = std::floor(x);
	*i = static_cast<int>(s);
	int offset = -iLow;
	iLow += offset;
	iHigh += offset;

	if (iLow == iHigh) {
		*i = iLow;
		*f = 0;
	}
	else if (*i < iLow) {
		*i = iLow;
		*f = 0;
	}
	else if (*i > iHigh - 1) {
		*i = iHigh - 1;
		*f = 1;
	}
	else {
		//*f为cell内的相对坐标
		*f = static_cast<double>(x - s);
	}
	*i -= offset;

}

//单线性插值
template<class T>
inline T lerp(const T& value0, const T& value1, double f) {
	return (1 - f) * value0 + f * value1;

}


//双线性插值
template<class T>
inline T bilerp(
	const T& f00,
	const T& f10,
	const T& f01,
	const T& f11,
	double tx, double ty) {
	return lerp(
		lerp(f00, f10, tx),
		lerp(f01, f11, tx),
		ty);

}


#endif