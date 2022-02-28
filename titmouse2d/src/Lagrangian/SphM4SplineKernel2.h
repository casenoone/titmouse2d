#ifndef SPHM4SPLINEKERNEL2_H
#define SPHM4SPLINEKERNEL2_H

#include "../Vector2.hpp"
#include "../Array.hpp"
#include "../ConstVar.h"

struct SphM4SplineKernel2 {
	double halfH, halfH2, halfH3, halfH4, halfH5,
		kernelValueCoeff, kernelGradientCoeffA, kernelGradientCoeffB,
		kernelLaplacianCoeffA, kernelLaplacianCoeffB;

	SphM4SplineKernel2();
	explicit SphM4SplineKernel2(double h_);
	double operator()(double distance)const;
	Vector2D gradient(double distance, const Vector2D& direction)const;

};

inline SphM4SplineKernel2::SphM4SplineKernel2() :
	halfH(0.0), halfH2(0.0), halfH3(0.0),
	halfH4(0.0), halfH5(0.0) {

	kernelValueCoeff = 0.0;

	kernelGradientCoeffA = 0.0;
	kernelGradientCoeffB = 0.0;

	kernelLaplacianCoeffA = 0.0;
	kernelLaplacianCoeffB = 0.0;
}

inline SphM4SplineKernel2::SphM4SplineKernel2(double h_)
	: halfH(h_ / 2.0), halfH2(halfH* halfH),
	halfH3(halfH* halfH2), halfH4(halfH2* halfH2),
	halfH5(halfH2* halfH3) {

	kernelValueCoeff = 1.0 / (4.0 * kPiD * halfH3);

	kernelGradientCoeffA = 3.0 / (4.0 * kPiD * halfH4);
	kernelGradientCoeffB = -3.0 / (4.0 * kPiD * halfH4);

	kernelLaplacianCoeffA = -9.0 / (kPiD * halfH5);
	kernelLaplacianCoeffB = 3.0 / (kPiD * halfH5);
}

inline double SphM4SplineKernel2::operator()(double distance)const {
	double q = distance / halfH;

	auto t1 = 2.0 - q;
	auto t2 = 1.0 - q;
	auto temp1 = t1 * t1 * t1;
	auto temp2 = t2 * t2 * t2;

	if (q < 1.0) {

		return kernelValueCoeff * (temp1 - 4 * temp2);
	}
	else {
		return kernelValueCoeff * temp1;
	}
}


inline Vector2D SphM4SplineKernel2::gradient(double distance, const Vector2D& direction)const {
	double q = distance / halfH;
	auto gradient = direction;
	if (q <= 0.0) {
		gradient = Vector2D();
	}
	else if (q < 1.0) {
		auto t1 = 1.0 - q;
		auto t2 = 2.0 - q;
		auto temp1 = t1 * t1;
		auto temp2 = t2 * t2;
		gradient = 1.0 * gradient * (kernelGradientCoeffA * (4.0 * temp1 - temp2) / distance);
	}
	else {
		auto t2 = 2.0 - q;
		auto temp2 = t2 * t2;
		gradient = 1.0 * gradient * ((kernelGradientCoeffB * temp2) / distance);
	}

	return gradient;

}
#endif