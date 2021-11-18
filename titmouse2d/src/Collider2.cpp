#include "Collider2.h"
#include "ConstVar.h"

Collider2::Collider2() {

}

Collider2::~Collider2() {

}

void Collider2::push(const Surface2Ptr& _surface) {
	_surfaces.push_back(_surface);
}

void Collider2::getClosestPoint(
	const Vector2<double>& queryPoint,
	ColliderQueryResult* result) const {

	auto minDis = kMaxD;

}


void Collider2::resolveCollision(
	double radius,
	double restitutionCoefficient,
	Vector2<double>* position,
	Vector2<double>* velocity
) {

	ColliderQueryResult colliderPoint;

	getClosestPoint(*position, &colliderPoint);
}



bool Collider2::isPenetrating(
	const Collider2::ColliderQueryResult& colliderPoint,
	const Vector2<double>& position,
	double radius) {

	return false;

}