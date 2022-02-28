#ifndef HEART2_H
#define HEART2_H

#include "ImplicitSurface2.h"

class Heart2 : public ImplicitSurface2 {
public:
	Heart2() = default;

	Heart2(const Vector2D& center, const double& r,
		const Vector2I& resolution,
		const Vector2D& origin,
		double initialValue);

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint) override;

	virtual const VertexCenteredScalarGrid2Ptr sdf() const;

public:


public:
	virtual void computeSdf()override;
};


#endif