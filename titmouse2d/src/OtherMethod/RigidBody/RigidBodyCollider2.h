#pragma once
#include <iostream>
#include <vector>
#include "../../Geometry/Surface2.h"

//Impluse method

class RigidBodyCollider2 {
public:
	RigidBodyCollider2() = default;

	void push(const Surface2Ptr& _surface) {
		surfaces.push_back(_surface);
	}

	void resolveCollision(
		double radius,
		double restitutionCoefficient,
		const Vector2D& position,
		const Vector2D& velocity,
		Vector2D& newVelocity
	);

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

public:
	std::vector<Surface2Ptr> surfaces;

private:
	double _frictionCoeffient = 0.0;

};

using RigidBodyCollider2Ptr = std::shared_ptr<RigidBodyCollider2>;
