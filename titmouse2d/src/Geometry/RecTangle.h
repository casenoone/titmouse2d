#pragma once

#include "Box2.h"
#include "../Vector2.hpp"
#include "../Matrix2x2.hpp"
#include "../Matrix3x3.hpp"

//�������дһƪ�����ܽ�һ��

class RecTangle : public ExplicitSurface2 {
public:
	RecTangle(const Box2& box) {

		local_upper = box.upperCorner - box.lowerCorner;
		local_x = Vector2D(1, 0);
		local_o = box.lowerCorner;

		for (int i = 0; i < 4; ++i) {
			auto start = box.lookAt(i).end;
			auto end = box.lookAt(i).start;
			auto normal = box.lookAt(i).normal;

			auto line = end - start;
			auto length = line.getLength();
			line = line.getNormalize();

			double len = length / (interval);
			for (int j = 0; j < interval; ++j) {

				SurfaceElement2 temp1;
				temp1.start = start + j * len * line;
				temp1.end = start + (j + 1.0) * len * line;
				temp1.normal = normal;

				_data.push_back(temp1);
			}
		}

		//����centerֵ
		_center = 0.5 * (box.upperCorner + box.lowerCorner);
	}

	void setRotationMatrix(const Matrix2x2<double>& mat) {
		rotationMat = mat;
	}

	Vector2D center()const {
		return _center;
	}

	bool IsInBox(const Vector2D& p) {

		auto p1 = toLocalMatrix * p;
		if (p1.x > local_lower.x && p1.x < local_upper.x
			&& p1.y > local_lower.y && p1.y < local_upper.y) {
			return true;
		}
		return false;
	}

	void updatePosition(double dt) {
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start += velocity * dt;
			i->end += velocity * dt;
		}
		_center += velocity * dt;
		local_o += velocity * dt;
	}

	//��������ת
	void updatePosition(double dt, const Matrix2x2<double>& mat) {

		//����ϵ��ת
		local_x = mat * local_x;
		//���¾ֲ�����ϵ����
		updateToLocalMatrix();

		//�ȰѶ���ƽ�ƻ�ԭ��
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start -= _center;
			i->end -= _center;
		}
		local_o -= _center;


		//Ȼ���ٿ�ʼ��ת
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start = mat * i->start;
			i->end = mat * i->end;
		}
		local_o = mat * local_o;
		//����ٰѶ���ƽ�ƻ�ȥ
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start += _center;
			i->end += _center;
		}
		local_o += _center;
		updatePosition(dt);
	}

	void updateVelocity(double dt, const Vector2D& force) {
		velocity += dt * force;
	}

	void updateToLocalMatrix() {
		auto world_x = Vector2D(1, 0);
		auto crossResult = world_x.cross(local_x);
		//����Ƕ�
		auto angle = std::abs(std::asin(crossResult / (world_x.dot(local_x))));
		if (crossResult < 0)angle *= -1;

		//�����ת����
		auto rotationM = Matrix3x3<double>::rotationMatrix(-angle);
		//��ƽ�ƾ���
		auto transM = Matrix3x3<double>::translatonMatrix(-local_o.x, -local_o.y);
		//auto transM = Matrix3x3<double>::translatonMatrix(-_center.x, -_center.y);
		//std::cout << local_o.x << "    " << local_o.y << std::endl;
		toLocalMatrix = rotationM * transM;
	}

public:
	Matrix2x2<double> rotationMat;
	Vector2D local_upper;
	Vector2D local_lower;

	Vector2D local_x;
	Vector2D local_o;

private:
	int interval = 13;
	Vector2D _center;

	//��������תʱ������
	Matrix3x3<double> toLocalMatrix;

};

using RecTanglePtr = std::shared_ptr<RecTangle>;


