#include "Sphere2.h"

Sphere2::Sphere2():
_center(Vector2<double>()),_r(1.0)
{

}

Sphere2::~Sphere2() {

}

Sphere2::Sphere2(const Vector2<double>& center, const double& r):
_center(center),_r(r)
{

}

Sphere2::Sphere2(const VertexCenteredScalarGrid2& other):
ImplicitSurface2(other)
{


}
Surface2::SurfaceQueryResult Sphere2::getClosedInformation(const Vector2<double>& otherPoint) {
	SurfaceQueryResult a;
	return a;
}

Vector2<double> Sphere2::center() {
	return _center;
}

double Sphere2::r() {
	return _r;
}


void Sphere2::computeSdf() {

}