#include "ImplicitSurface2.h"


class Sphere2 : public ImplicitSurface2 {
public:
	Sphere2();

	~Sphere2();

	Sphere2(const Vector2<double>& center, const double& r,
		const Vector2<size_t>& resolution,
		const Vector2<double>& origin,
		double initialValue
		);

	Sphere2(const VertexCenteredScalarGrid2& other);

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint) override;

public:
	Vector2<double> center();

	double r();

public:
	virtual void computeSdf()override;

private:
	Vector2<double> _center;

	double _r;

};