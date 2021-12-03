#ifndef IMPLICITSURFACE2_H
#define IMPLICITSURFACE2_H

#include "Surface2.h"

#include "../Eulerian/VertexCenteredScalarGrid2.h"

class ImplicitSurface2 : public Surface2 {
public:
	ImplicitSurface2();

	~ImplicitSurface2();

	ImplicitSurface2(const VertexCenteredScalarGrid2& other);

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const final;

	virtual double closestDistance(const Vector2<double>& otherPoint)const final;

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint) = 0;

//暂时设为private属性
public:

	VertexCenteredScalarGrid2Ptr _data;

};



#endif