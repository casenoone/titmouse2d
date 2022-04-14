#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"
#include "Vector2.hpp"

#include <iostream>
#include<vector>

//ע������collider���÷�
//Ϊ��ʵ�ֶ�̬
//ֻ������pushָ�����͵�surface


//Ϊ�˼򻯣���������ʱ�������ƶ��߽�
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

	//��ѯ��ǰ�㵽�����������һ��
	void getClosestPoint(
		const Vector2D& queryPoint,
		ColliderQueryResult* result) const;

	//��������Ƿ�͸����
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