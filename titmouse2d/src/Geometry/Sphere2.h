#include "ImplicitSurface2.h"


class Sphere2 : public ImplicitSurface2 {
public:
	Sphere2();

	~Sphere2();

	Sphere2(const Vector2<double>& center, const double& r);

	Sphere2(const VertexCenteredScalarGrid2& other);

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint) override;

public:
	Vector2<double> center();

	double r();

public:
	void computeSdf();

private:
	Vector2<double> _center;

	double _r;

};