#include "ImplicitSurface2.h"
#include "../Eulerian/MarchingCubes2.h"

ImplicitSurface2::ImplicitSurface2() {
}

ImplicitSurface2::ImplicitSurface2(const Vector2I& resolution,
	const Vector2D& origin,
	double initialValue) {

	_data = std::make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);
}

ImplicitSurface2::~ImplicitSurface2() {

}

ImplicitSurface2::ImplicitSurface2(const VertexCenteredScalarGrid2& other) {
	//_data = make_shared<VertexCenteredScalarGrid2>(other);
}

Vector2D ImplicitSurface2::closestPoint(const Vector2D& otherPoint)const {
	return Vector2D(0.0, 0.0);
}

double ImplicitSurface2::closestDistance(const Vector2D& otherPoint)const {
	return 0.0;
}


Surface2::SurfaceQueryResult ImplicitSurface2::getClosedInformation(const Vector2D& otherPoint) {

	SurfaceQueryResult a;
	return a;
}


ExplicitSurface2Ptr ImplicitSurface2::transformToExplicitSurface()const {

	ExplicitSurface2Ptr result;

	auto resolution = _data->resolution();
	auto origin = _data->origin();

	auto mc = MarchingCube2::builder()
		.withResolution(resolution)
		.withOrigin(origin)
		.makeShared();

	std::vector<SurfaceElement2> surfaceSet;
	mc->getLineSegmentSet(surfaceSet, _data);
	result = std::make_shared<ExplicitSurface2>(surfaceSet);

	return result;

}