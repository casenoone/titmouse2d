#include "GridSystemData2.h"

GridSystemData2::GridSystemData2() {
}


GridSystemData2::~GridSystemData2() {
}


GridSystemData2::GridSystemData2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin) {
	_velocity = std::make_shared<FaceCenteredGrid2>(resolution, origin, Vector2D());

	resize(resolution, gridSpacing, origin);
}

GridSystemData2::GridSystemData2(const GridSystemData2& other) {

}


void GridSystemData2::resize(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin) {
	_resolution = resolution;
	_gridSpacing = gridSpacing;
	_origin = origin;

}



Vector2I GridSystemData2::resolution() const {
	return _resolution;
}

Vector2D GridSystemData2::gridSpacing() const {
	return _gridSpacing;
}

Vector2D GridSystemData2::origin() const {
	return _origin;
}


FaceCenteredGrid2Ptr& GridSystemData2::velocity() {
	return _velocity;
}


VertexCenteredScalarGrid2Ptr& GridSystemData2::sdf() {
	return _sdf;
}