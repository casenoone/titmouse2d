#include "Quaternion.h"

Matrix2x2<double> Quaternion::toMatrix() {
	auto s2 = s * s;
	auto z2 = v.z * v.z;
	auto x11 = s2 - z2;
	auto x12 = -2 * s * v.z;
	auto x21 = -x12;
	auto x22 = -x11;

	Matrix2x2<double> result(x11, x12, x21, x22);
	return result;
}