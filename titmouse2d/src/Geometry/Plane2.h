#ifndef PLANE2_H
#define PLANE2_H

#include "ExplicitSurface2.h"

//本框架不使用无限大的平面
//这里是有限大小的二维平面


class Plane2 : public ExplicitSurface2 {
public:
	Plane2();

	virtual ~Plane2();

	Plane2(const Vector2<double>& _ponit1,
		const Vector2<double>& _point2,
		bool isFliped = false);

	Plane2(const Plane2& other);

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const;

	virtual double closestDistance(const Vector2<double>& otherPoint)const;

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint);

	virtual void ghost() const {

	}

public:
	Vector2<double> point1;

	Vector2<double> point2;

	Vector2<double> normal;


};


using Plane2Ptr = shared_ptr<Plane2>;



#endif

