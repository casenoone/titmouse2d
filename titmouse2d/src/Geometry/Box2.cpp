#include "Box2.h"


Box2::Box2() {

}

Box2::~Box2() {

}

//box的顶点分布

/*****1*****/
/***********/
/*4*******2*/
/***********/
/*****3*****/

Box2::Box2(const Vector2<double>& _lowerCorner, 
	const Vector2<double>& _upperCorner):
	lowerCorner(_lowerCorner),upperCorner(_upperCorner)
{
	
	Vector2<double> A(lowerCorner.x, upperCorner.y);
	Vector2<double> B = upperCorner;
	Vector2<double> C(upperCorner.x, lowerCorner.y);
	Vector2<double> D = lowerCorner;
	
	SurfaceElement2 e1(A, B, Vector2<double>(0, 1));
	SurfaceElement2 e2(B, C, Vector2<double>(1, 0));
	SurfaceElement2 e3(C, D, Vector2<double>(0, -1));
	SurfaceElement2 e4(D, A, Vector2<double>(-1, 0));

	_data.push_back(e1);
	_data.push_back(e2);
	_data.push_back(e3);
	_data.push_back(e4);

}


Box2::Box2(const Box2& other) {
	*this = other;
}


Vector2<double> Box2::closestPoint(const Vector2<double>& otherPoint)const {
	return ExplicitSurface2::closestPoint(otherPoint);
}


double Box2::closestDistance(const Vector2<double>& otherPoint)const {
	return ExplicitSurface2::closestDistance(otherPoint);
}

Surface2::SurfaceQueryResult Box2::getClosedInformation(const Vector2<double>& otherPoint) {
	return ExplicitSurface2::getClosedInformation(otherPoint);
}