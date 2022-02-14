#include "ImplicitSurface2.h"

ImplicitSurface2::ImplicitSurface2() {
	//_data = make_shared<VertexCenteredScalarGrid2>();
}

ImplicitSurface2::~ImplicitSurface2() {

}

ImplicitSurface2::ImplicitSurface2(const VertexCenteredScalarGrid2& other) {
	//_data = make_shared<VertexCenteredScalarGrid2>(other);
}

Vector2<double> ImplicitSurface2::closestPoint(const Vector2<double>& otherPoint)const {
	return Vector2<double>(0.0, 0.0);
}

double ImplicitSurface2::closestDistance(const Vector2<double>& otherPoint)const {
	return 0.0;
}


Surface2::SurfaceQueryResult ImplicitSurface2::getClosedInformation(const Vector2<double>& otherPoint) {

	SurfaceQueryResult a;
	return a;
}

