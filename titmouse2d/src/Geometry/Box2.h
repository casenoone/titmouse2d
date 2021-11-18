#ifndef BOX2_H
#define BOX2_H

#include "ExplicitSurface2.h"

class Box2 : public ExplicitSurface2 {
public:
	Box2();

	virtual ~Box2();

	Box2(const Vector2<double>& _lowerCorner, const Vector2<double>& _upperCorner);

	Box2(const Box2& other);

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const;

	virtual double closestDistance(const Vector2<double>& otherPoint)const;

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint);

	virtual void ghost() const{
		
	}

public:
	Vector2<double> lowerCorner;

	Vector2<double> upperCorner;


};


using Box2Ptr = shared_ptr<Box2>;



#endif