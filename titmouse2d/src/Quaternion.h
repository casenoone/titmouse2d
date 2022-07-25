#pragma once
#include "Vector3.hpp"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

class Quaternion {
public:
	Quaternion() = default;
	//angleΪ��ת�Ƕ�
	//vΪ��ת��
	Quaternion(double angle, const Vector3D& v_) :
		s(std::cos(angle / 2)), v(v_) {}

	//��Ԫ��ת����
	Matrix3x3<double> toMatrix();

	//�ӷ�
	Quaternion operator+(const Quaternion& q) {
		Quaternion result(s + q.s, v + q.v);
		return result;
	}

	//���
	Quaternion cross(const Quaternion& q) {
		Quaternion result(s * q.s - v.dot(q.v), s * q.v + q.s * v + v.cross(q.v));
		return result;
	}

	//��һ��
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