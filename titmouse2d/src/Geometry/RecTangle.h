#pragma once

#include "Box2.h"
#include "../Vector2.hpp"
#include "../Matrix2x2.hpp"

class RecTangle : public ExplicitSurface2 {
public:
	RecTangle(const Box2& box) :upper(box.upperCorner), lower(box.lowerCorner) {
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

		//计算center值
		_center = 0.5 * (box.upperCorner + box.lowerCorner);

	}

	void setRotationMatrix(const Matrix2x2<double>& mat) {
		rotationMat = mat;
	}

	Vector2D center()const {
		return _center;
	}

	bool IsInBox(const Vector2D& p) {
		if (p.x <= upper.x &&
			p.x >= lower.x &&
			p.y <= upper.y &&
			p.y >= lower.y) {
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

		//更新角点位置
		upper += velocity * dt;
		lower += velocity * dt;
	}

	//绕自身旋转
	void updatePosition(double dt, const Matrix2x2<double>& mat) {
		//先把顶点平移回原点
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start -= _center;
			i->end -= _center;
			upper -= _center;
			lower -= _center;
		}

		//然后再开始旋转
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start = mat * i->start;
			i->end = mat * i->end;
			upper = mat * upper;
			lower = mat * lower;
		}

		//最后再把顶点平移回去
		for (auto i = _data.begin(); i != _data.end(); ++i) {
			i->start += _center;
			i->end += _center;
			upper -= _center;
			lower -= _center;
		}

		updatePosition(dt);
	}

	void updateVelocity(double dt, const Vector2D& force) {
		velocity += dt * force;
	}

public:
	Matrix2x2<double> rotationMat;
	Vector2D upper;
	Vector2D lower;

private:
	int interval = 13;
	Vector2D _center;


};

using RecTanglePtr = std::shared_ptr<RecTangle>;


