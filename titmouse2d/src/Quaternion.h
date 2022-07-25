#pragma once
#include "Vector3.hpp"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

class Quaternion {
public:
	Quaternion() = default;
	//angle为旋转角度
	//v为旋转轴
	Quaternion(double angle, const Vector3D& v_) :
		s(std::cos(angle / 2)), v(v_) {}

	//四元数转矩阵
	Matrix3x3<double> toMatrix();

	//加法
	Quaternion operator+(const Quaternion& q) {
		Quaternion result(s + q.s, v + q.v);
		return result;
	}

	//叉积
	Quaternion cross(const Quaternion& q) {
		Quaternion result(s * q.s - v.dot(q.v), s * q.v + q.s * v + v.cross(q.v));
		return result;
	}

	//归一化
	void normalized() {
		auto s2 = s * s;
		auto v2 = v.getLengthSquared();
		auto len = std::sqrt(s2 + v2);
		s /= len;
		v /= len;
	}

public:
	double s = 0;
	Vector3D v;
};