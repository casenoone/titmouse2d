#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"
#include "Vector2.hpp"

#include <iostream>
#include<vector>

//注意这里collider的用法
//为了实现多态
//只能往里push指针类型的surface


//为了简化，这里先暂时不考虑移动边界
class Collider2 {
public:
	Collider2();

	~Collider2();

	void push(const Surface2Ptr& _surface);

	void resolveCollision(
		double radius,
		double restitutionCoefficient,
		Vector2D* position,
		Vector2D* velocity
	);

	bool IsNull()const;

public:
	std::vector<Surface2Ptr> _surfaces;

	struct ColliderQueryResult final {
		double distance;
		Vector2D point;
		Vector2D normal;
		Vector2D velocity;
	};

	//查询当前点到表面上最近的一点
	void getClosestPoint(
		const Vector2D& queryPoint,
		ColliderQueryResult* result) const;

	//检查粒子是否穿透表面
	bool isPenetrating(
		const ColliderQueryResult& colliderPoint,
		const Vector2D& position,
		double radius);

private:
	double _frictionCoeffient = 0.0;
};

inline bool Collider2::IsNull()const {
	if (_surfaces.empty())return true;
	return false;
}

#endif