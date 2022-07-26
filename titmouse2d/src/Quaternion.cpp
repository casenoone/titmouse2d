#include "Quaternion.h"

Matrix3x3<double> Quaternion::toMatrix() {
	auto s2 = s * s;
	auto x2 = v.x * v.x;
	auto y2 = v.y * v.y;
	auto z2 = v.z * v.z;
	auto xy = v.x * v.y;
	auto sz = s * v.z;
	auto sy = s * v.y;
	auto yz = v.y * v.z;
	auto sx = s * v.x;
	auto xz = v.x * v.z;

	auto x11 = s2 + x2 - y2 - z2;
	auto x12 = 2 * (xy - sz);
	auto x13 = 2 * (xz + sy);
	auto x21 = 2 * (xy + sz);
	auto x22 = s2 - x2 + y2 - z2;
	auto x23 = 2 * (yz - sx);
	auto x31 = 2 * (xz - sy);
	auto x32 = 2 * (yz + sx);
	auto x33 = s2 - x2 - y2 + z2;

	Matrix3x3<double> result(x11, x12, x13, x21, x22, x23, x31, x32, x33);
	if (result.det() == 0) {
		return Matrix3x3<double>::identityMatrix();
	}
	return result;
}