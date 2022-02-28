#ifndef SPHSPIKYKERNEL2_H
#define SPHSPIKYKERNEL2_H

#include "../Vector2.hpp"
#include "../Array2.hpp"
#include "../ConstVar.h"

struct SphSpikyKernel2 {
	double h, h2, h3, h4, h5;
	SphSpikyKernel2();
	explicit SphSpikyKernel2(double kernelRadius);
	double operator()(double distance)const;
	double firstDerivative(double distance)const;
	Vector2D gradient(double distance, const Vector2D& direction)const;
	double secondDerivative(double distance)const;
};

inline SphSpikyKernel2::SphSpikyKernel2()
	:h(0), h2(0), h3(0), h4(0), h5(0) {}

inline SphSpikyKernel2::SphSpikyKernel2(double h_)
	: h(h_), h2(h* h), h3(h2* h), h4(h2* h2), h5(h3* h2) {}

inline double SphSpikyKernel2::operator()(double distance)const {
	if (distance >= h) {
		return 0.0;
	}
	else {
		double x = 1.0 - distance / h;
		return 15.0 / (kPiD * h3) * x * x * x;
	}
}

inline double SphSpikyKernel2::firstDerivative(double distance)const {
	if (distance >= h || distance <= 0.0) {
		return 0.0;
	}
	else {
		//double x = 1.0 - distance / h;

		//return -45.0 / (kPiD * h4) * x * x;
		double x = (h - distance);
		return (-45.0 / (kPiD * h5 * h)) * x * x;

	}
}

inline Vector2D SphSpikyKernel2::gradient(double distance, const Vector2D& directionToCenter)const {
	return directionToCenter * -1 * firstDerivative(distance);
}

inline double SphSpikyKernel2::secondDerivative(double distance)const {
	if (distance >= h) {
		return 0.0;
	}
	else {
		double x = 1.0 - distance / h;
		return 90.0 / (kPiD * h5) * x;
	}
}

#endif