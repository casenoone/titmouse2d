#ifndef SPHPOLY6KERNEL2_H
#define SPHPOLY6KERNEL2_H

#include "../Vector2.hpp"
#include "../Array2.hpp"
#include "../ConstVar.h"

struct SphPolyKernel2 {
	double h, h2, h8;
	SphPolyKernel2() = default;
	explicit SphPolyKernel2(double kernelRadius);
	double operator()(double distance)const;
	Vector2D gradient(double distance, const Vector2D& direction)const;
	double laplace(double distance, const Vector2D& direction) const;


};


inline SphPolyKernel2::SphPolyKernel2(double h_) :h(h_), h2(h_* h_), h8(h2* h2* h2* h2) {

}
inline double SphPolyKernel2::operator()(double distance)const {
	if (distance >= h || distance <= 0) {
		return 0.0;
	}
	else {
		auto alpha = 4 / (kPiD * h8);
		auto h2_r2 = h2 - distance * distance;
		return alpha * std::pow(h2_r2, 3);
	}
}
inline Vector2D SphPolyKernel2::gradient(double distance, const Vector2D& direction)const {
	if (distance >= h) {
		return Vector2D(0.0, 0.0);
	}
	else {
		auto alpha = -24 / (kPiD * h8);
		auto h2_r2 = h2 - distance * distance;
		return alpha * std::pow(h2_r2, 2) * direction;
	}
}
inline double SphPolyKernel2::laplace(double distance, const Vector2D& direction) const {
	if (distance >= h) {
		return 0.0;
	}
	else {
		auto alpha = -24 / (kPiD * h8);
		auto r2 = distance * distance;
		auto h2_r2 = h2 - r2;
		return 3 * std::pow(h2_r2, 2) - 4 * r2 * h2_r2;

	}
}

#endif