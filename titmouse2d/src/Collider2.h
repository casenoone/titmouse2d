#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"
#include "Vector2.hpp"

#include <iostream>
using namespace std;
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
		Vector2<double>* position,
		Vector2<double>* velocity
	);

	bool IsNull()const;

public:
	vector<Surface2Ptr> _surfaces;

	struct ColliderQueryResult final {
		double distance;
		Vector2<double> point;
		Vector2<double> normal;
		Vector2<double> velocity;
	};

	//��ѯ��ǰ�㵽�����������һ��
	void getClosestPoint(
		const Vector2<double>& queryPoint,
		ColliderQueryResult* result) const;

	//��������Ƿ�͸����
	bool isPenetrating(
		const ColliderQueryResult& colliderPoint,
		const Vector2<double>& position,
		double radius);

private:
	double _frictionCoeffient = 0.0;
};

inline bool Collider2::IsNull()const {
	if (_surfaces.empty())return true;
	return false;
}

#endif