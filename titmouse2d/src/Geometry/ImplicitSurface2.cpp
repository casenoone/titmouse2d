#include "ImplicitSurface2.h"
#include "../Eulerian/MarchingCubes2.h"

ImplicitSurface2::ImplicitSurface2() {
}

ImplicitSurface2::ImplicitSurface2(const Vector2<size_t>& resolution,
	const Vector2<double>& origin,
	double initialValue) {

	_data = make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);
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


ExplicitSurface2Ptr ImplicitSurface2::transformToExplicitSurface()const {

	ExplicitSurface2Ptr result;

	auto resolution = _data->resolution();
	auto origin = _data->origin();

	auto mc = MarchingCube2::builder()
		.withResolution(resolution)
		.withOrigin(origin)
		.makeShared();

	vector<SurfaceElement2> surfaceSet;
	mc->getLineSegmentSet(surfaceSet, _data);
	result = make_shared<ExplicitSurface2>(surfaceSet);

	return result;

}