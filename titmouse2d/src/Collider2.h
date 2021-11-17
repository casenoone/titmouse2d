#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"
#include "Vector2.hpp"

#include <iostream>
using namespace std;
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
		Vector2<double>* position,
		Vector2<double>* velocity
	);


public:
	vector<Surface2Ptr> _surfaces;

	struct ColliderQueryResult final {
		double distance;
		Vector2<double> point;
		Vector2<double> normal;
		Vector2<double> velocity;
	};

	//查询当前点到表面上最近的一点
	void getClosestPoint(
		const Vector2<double>& queryPoint,
		ColliderQueryResult* result) const;

	//检查粒子是否穿透表面
	bool isPenetrating(
		const ColliderQueryResult& colliderPoint,
		const Vector2<double>& position,
		double radius);

};


#endif