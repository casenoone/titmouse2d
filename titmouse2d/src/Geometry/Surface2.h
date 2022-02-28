#ifndef SURFACE2_H
#define SURFACE2_H

#include <iostream>
using namespace std;

#include "../Vector2.hpp"

class Surface2 {
public:
	Surface2();

	virtual ~Surface2();

	virtual Vector2D closestPoint(const Vector2D& otherPoint)const = 0;

	virtual double closestDistance(const Vector2D& otherPoint)const = 0;

public:
	struct SurfaceQueryResult final {
		double distance;
		Vector2D point;
		Vector2D normal;
		Vector2D velocity;
	};


	SurfaceQueryResult _surfaceQueryResult;

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint) = 0;

};

using Surface2Ptr = shared_ptr<Surface2>;


#endif