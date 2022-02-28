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

Box2::Box2(const Vector2D& _lowerCorner,
	const Vector2D& _upperCorner,
	bool IsFliped) :
	lowerCorner(_lowerCorner), upperCorner(_upperCorner)
{

	Vector2D A(lowerCorner.x, upperCorner.y);
	Vector2D B = upperCorner;
	Vector2D C(upperCorner.x, lowerCorner.y);
	Vector2D D = lowerCorner;

	if (IsFliped) {
		SurfaceElement2 e1(A, B, Vector2D(0, -1));
		SurfaceElement2 e2(B, C, Vector2D(-1, 0));
		SurfaceElement2 e3(C, D, Vector2D(0, 1));
		SurfaceElement2 e4(D, A, Vector2D(1, 0));

		_data.push_back(e1);
		_data.push_back(e2);
		_data.push_back(e3);
		_data.push_back(e4);
	}
	else {
		SurfaceElement2 e1(A, B, Vector2D(0, 1));
		SurfaceElement2 e2(B, C, Vector2D(1, 0));
		SurfaceElement2 e3(C, D, Vector2D(0, -1));
		SurfaceElement2 e4(D, A, Vector2D(-1, 0));

		_data.push_back(e1);
		_data.push_back(e2);
		_data.push_back(e3);
		_data.push_back(e4);
	}




}


Box2::Box2(const Box2& other) {
	*this = other;
}


Vector2D Box2::closestPoint(const Vector2D& otherPoint)const {

	return ExplicitSurface2::closestPoint(otherPoint);
}


double Box2::closestDistance(const Vector2D& otherPoint)const {
	return ExplicitSurface2::closestDistance(otherPoint);
}



Surface2::SurfaceQueryResult Box2::getClosedInformation(const Vector2D& otherPoint) {
	return ExplicitSurface2::getClosedInformation(otherPoint);
}