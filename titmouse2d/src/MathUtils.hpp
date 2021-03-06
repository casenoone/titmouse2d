#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <iostream>

#include <cmath>
#include "ConstVar.h"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

//这里直接Ctrl CV了 doyub kim的代码
//仅改了数据类型

//二维向量的外积
template<class T>
Matrix2x2<T> crossProduct(const Vector2<T>& a, const Vector2<T>& b) {
	auto x11 = a.x * b.x;
	auto x12 = a.x * b.y;
	auto x21 = a.y * b.x;
	auto x22 = a.y * b.y;

	Matrix2x2<T> result(x11, x12, x21, x22);
	return result;
}

//三维向量的外积
template<class T>
Matrix3x3<T> crossProduct(const Vector3<T>& a, const Vector3<T>& b) {
	auto x11 = a.x * b.x;
	auto x12 = a.x * b.y;
	auto x13 = a.x * b.z;
	auto x21 = a.y * b.x;
	auto x22 = a.y * b.y;
	auto x23 = a.y * b.z;
	auto x31 = a.z * b.x;
	auto x32 = a.z * b.y;
	auto x33 = a.z * b.z;

	Matrix3x3<T> result(x11, x12, x13, x21, x22, x23, x31, x32, x33);
	return result;
}


//计算平方
template<class T>
inline T sqr(const T& x)
{
	return x * x;
}

template <typename T>
inline T square(T x) {
	return x * x;
}

template <typename T>
inline T cubic(T x) {
	return x * x * x;
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


template <typename T>
inline T sign(T x) {
	if (x >= 0) {
		return static_cast<T>(1);
	}
	else {
		return static_cast<T>(-1);
	}
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



//四元数相关操作




#endif