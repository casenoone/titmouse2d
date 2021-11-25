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

	//cout << result->normal.x << " " << result->normal.y << endl;

}


void Collider2::resolveCollision(
	double radius,
	double restitutionCoefficient,
	Vector2<double>* position,
	Vector2<double>* velocity
) {

	ColliderQueryResult colliderPoint;

	getClosestPoint(*position, &colliderPoint);
	//cout << colliderPoint.distance << endl;
	if (isPenetrating(colliderPoint, *position, 0.00001)) {
		

		Vector2<double> targetNormal = colliderPoint.normal;
		Vector2<double> targetPoint = colliderPoint.point + targetNormal * radius;
		Vector2<double> colliderVelAtTargetPoint = colliderPoint.velocity;

		Vector2<double> relativeVel = *velocity - colliderVelAtTargetPoint;

		double normalDotRelativeVel = targetNormal.dot(relativeVel);

		//相对速度的法线分量
		//相对速度的切线分量
		Vector2<double> relativeVelN = targetNormal * normalDotRelativeVel;
		Vector2<double> relativeVelT = relativeVel - relativeVelN;

		if (normalDotRelativeVel < 0.0) {
			Vector2<double> deltaRelativeVelN =
				relativeVelN * (-restitutionCoefficient - 1.0);
			relativeVelN = relativeVelN * -restitutionCoefficient;

			//Bridson et al., Robust Treatment of Collisions, Contact and
			// Friction for Cloth Animation, 2002
			if (relativeVelT.getLengthSquared() > 0.0) {
				double frictionScale = std::max(
					1.0 - _frictionCoeffient * deltaRelativeVelN.getLength() /
					relativeVelT.getLength(),
					0.0);
				relativeVelT = relativeVelT * frictionScale;
			}

			//合成最终速度
			*velocity =
				relativeVelN + relativeVelT + colliderVelAtTargetPoint;
		}

		*position = targetPoint;
		
	}

}


//由最近点到粒子点引一个向量
//判断这个向量与法向量的点积
bool Collider2::isPenetrating(
	const Collider2::ColliderQueryResult& colliderPoint,
	const Vector2<double>& position,
	double radius) {

	auto OP = position - colliderPoint.point;
	auto result = OP.dot(colliderPoint.normal);

	if (result < 0 && OP.getLengthSquared()<0.01) {
		return true;
	}


	return false;

}