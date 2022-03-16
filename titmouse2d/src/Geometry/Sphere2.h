#ifndef SPHERE2_H
#define SPHERE2_H


#include "ImplicitSurface2.h"


class Sphere2 : public ImplicitSurface2 {
public:
	Sphere2();

	~Sphere2();

	Sphere2(const Vector2D& center, const double& r,
		const Vector2I& resolution,
		const Vector2D& origin = Vector2D(),
		double initialValue = 0.0
	);

	Sphere2(const VertexCenteredScalarGrid2& other);

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint) override;

	virtual const VertexCenteredScalarGrid2Ptr sdf() const;

	BoundingBox2 boundingBox()const override;

	BoundingBox2 computeBoundingBox()override;


public:
	Vector2D center();

	double r();

public:
	virtual void computeSdf()override;

private:
	Vector2D _center;

	double _r;

};

inline BoundingBox2 Sphere2::boundingBox()const {

	return _boundingBox;
}

inline BoundingBox2 Sphere2::computeBoundingBox() {

	_boundingBox.lowerCorner = Vector2D(_center.x - _r, _center.y - _r);
	_boundingBox.upperCorner = Vector2D(_center.x + _r, _center.y + _r);
	return _boundingBox;
}

#endif