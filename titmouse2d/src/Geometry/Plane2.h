#ifndef PLANE2_H
#define PLANE2_H

#include "ExplicitSurface2.h"

//本框架不使用无限大的平面
//这里是有限大小的二维平面


class Plane2 : public ExplicitSurface2 {
public:
	Plane2();

	virtual ~Plane2();

	Plane2(const Vector2D& _ponit1,
		const Vector2D& _point2,
		bool isFliped = false);

	Plane2(const Plane2& other);

	virtual Vector2D closestPoint(const Vector2D& otherPoint)const;

	virtual double closestDistance(const Vector2D& otherPoint)const;


	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint);


public:
	Vector2D point1;

	Vector2D point2;

	Vector2D normal;


};


using Plane2Ptr = std::shared_ptr<Plane2>;



#endif

