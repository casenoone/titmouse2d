#ifndef SPHSTDKERNEL2_H
#define SPHSTDKERNEL2_H

#include "../ConstVar.h"


struct SphStdKernel2 {
	double h, h2, h3, h5;
	SphStdKernel2();
	SphStdKernel2(double kernelRadius);
	double operator()(double distance)const;
	double firstDerivative(double distance)const;
	Vector2D gradient(double distance, const Vector2D& direction)const;
	double secondDerivative(double distance)const;

};

inline SphStdKernel2::SphStdKernel2()
	: h(0), h2(0), h3(0), h5(0) {}

inline SphStdKernel2::SphStdKernel2(double kernelRadius)
	: h(kernelRadius), h2(h* h), h3(h2* h), h5(h3* h2) {}

inline double SphStdKernel2 :: operator()(double distance)const {
	if (distance * distance >= h2 || distance <= 0) {
		return 0.0;
	}
	else {
		double x = 1.0 - distance * distance / h2;
		return 315.0 / (64.0 * kPiD * h3) * x * x * x;
	}
}

inline double SphStdKernel2::firstDerivative(double distance)const {
	if (distance >= h) {
		return 0.0;
	}
	else {
		double x = 1.0 - distance * distance / h2;
		return -945.0 / (32.0 * kPiD * h5) * distance * x * x;
	}
}

inline Vector2D SphStdKernel2::gradient(double distance, const Vector2D& directionToCenter)const {
	return directionToCenter * firstDerivative(distance) * -1;
}

inline double SphStdKernel2::secondDerivative(double distance)const {
	if (distance * distance >= h2) {
		return 0.0;
	}
	else {
		double x = distance * distance / h2;

		//书上的代码有错误
		//应该是(5 * x - 1)
		//而不是(3 * x - 1)
		return 945.0 / (32.0 * kPiD * h5) * (1 - x) * (5 * x - 1);
	}
}

#endif