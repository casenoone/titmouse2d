#include "EulerianSmokeSolver2.h"


void EulerianSmokeSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {



}



EulerianSmokeSolver2::Builder EulerianSmokeSolver2::builder() {
	return Builder();
}

EulerianSmokeSolver2 EulerianSmokeSolver2::Builder::build() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return EulerianSmokeSolver2(_resolution, gridSpacing, _gridOrigin);
}


EulerianSmokeSolver2Ptr EulerianSmokeSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<EulerianSmokeSolver2>(
		new EulerianSmokeSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](EulerianSmokeSolver2* obj) {
			delete obj;
		});
}