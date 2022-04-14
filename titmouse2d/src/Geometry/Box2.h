#ifndef BOX2_H
#define BOX2_H

#include "ExplicitSurface2.h"

class Box2 : public ExplicitSurface2 {
public:
	Box2();

	virtual ~Box2();

	Box2(const Vector2D& _lowerCorner, const Vector2D& _upperCorner, bool IsFliped = false);

	Box2(const Box2& other);

	virtual Vector2D closestPoint(const Vector2D& otherPoint)const;

	virtual double closestDistance(const Vector2D& otherPoint)const;

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint);



public:
	Vector2D lowerCorner;

	Vector2D upperCorner;


};


using Box2Ptr = std::shared_ptr<Box2>;



#endif