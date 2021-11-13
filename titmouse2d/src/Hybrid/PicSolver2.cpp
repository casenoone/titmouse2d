#include "PicSolver2.h"

PicSolver2::PicSolver2() {
}


PicSolver2::PicSolver2(
	const Vector2& resolution,
	const Vector2& gridSpacing,
	const Vector2& gridOrigin)
	: GridFluidSolver2(resolution, gridSpacing, gridOrigin) {
	auto grids = gridSystemData();

	_particles = std::make_shared<ParticleSystemData2>();
}



PicSolver2::Builder PicSolver2::builder() {
	return Builder();
}


PicSolver2 PicSolver2::Builder::build() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return PicSolver2(_resolution, gridSpacing, _gridOrigin);
}


PicSolver2Ptr PicSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<PicSolver2>(
		new PicSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](PicSolver2* obj) {
			delete obj;
		});
}