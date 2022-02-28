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

public:
	Vector2D center();

	double r();

public:
	virtual void computeSdf()override;

private:
	Vector2D _center;

	double _r;

};



#endif