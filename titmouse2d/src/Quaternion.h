#pragma once
#include "Vector3.hpp"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

class Quaternion {
public:
	Quaternion() = default;
	//angleΪ��ת�Ƕ�
	//vΪ��ת��
	Quaternion(double s_, const Vector3D& v_) :
		s(s_), v(v_) {}

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
		auto v2 = v.dot(v);
		auto len = std::sqrt(s2 + v2);
		if (len) {
			s /= len;
			v /= len;
		}
	}

public:
	double s = 1;
	Vector3D v;
};