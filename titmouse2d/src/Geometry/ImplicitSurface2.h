#ifndef IMPLICITSURFACE2_H
#define IMPLICITSURFACE2_H

#include "Surface2.h"
#include "../Eulerian/VertexCenteredScalarGrid2.h"
#include "ExplicitSurface2.h"


class ImplicitSurface2 : public Surface2 {
public:
	ImplicitSurface2();

	ImplicitSurface2(const Vector2I& resolution,
		const Vector2D& origin,
		double initialValue);

	~ImplicitSurface2();

	ImplicitSurface2(const VertexCenteredScalarGrid2& other);

	virtual Vector2D closestPoint(const Vector2D& otherPoint)const final;

	virtual double closestDistance(const Vector2D& otherPoint)const final;

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint) = 0;

	virtual const VertexCenteredScalarGrid2Ptr sdf() const = 0;

	virtual void computeSdf() = 0;

	ExplicitSurface2Ptr transformToExplicitSurface()const;

	virtual BoundingBox2 boundingBox()const {
		return BoundingBox2(Vector2D(), Vector2D());
	}

	virtual BoundingBox2 computeBoundingBox() {
		return BoundingBox2(Vector2D(), Vector2D());
	}

public:

	VertexCenteredScalarGrid2Ptr _data;
	BoundingBox2 _boundingBox;
};



#endif