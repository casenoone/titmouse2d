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
	Surface2::SurfaceQueryResult minSurfaceInformation;
	auto resultIter = _surfaces.begin();
	for (auto i = _surfaces.begin(); i != _surfaces.end(); ++i) {
		(*i)->getClosedInformation(queryPoint);
		auto surfaceInformation = (*i)->_surfaceQueryResult;
		if (surfaceInformation.distance < minDis) {
			minDis = surfaceInformation.distance;
			resultIter = i;
		}
	}

	result->distance = (*resultIter)->_surfaceQueryResult.distance;
	result->normal   = (*resultIter)->_surfaceQueryResult.normal;
	result->point    = (*resultIter)->_surfaceQueryResult.point;
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


//由最近点到粒子点引一个向量
//判断这个向量与法向量的点积
bool Collider2::isPenetrating(
	const Collider2::ColliderQueryResult& colliderPoint,
	const Vector2<double>& position,
	double radius) {

	auto OP = position - colliderPoint.point;
	auto result = OP.dot(colliderPoint.normal);
	if (result < 0) {
		return true;
	}

	return false;

}