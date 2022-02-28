#include "EulerianSmokeSolver2.h"

#include "../../random.h"

void EulerianSmokeSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {

	addDensity();
	addTemperature();
	computeExternalForces(timeIntervalInSeconds);
	computeAdvection(timeIntervalInSeconds);

	setMarkers();
	setFluidCellNum();
	computePressure(timeIntervalInSeconds);

}

void EulerianSmokeSolver2::addDensity() {
	Vector2I lower(1, 3);
	Vector2I upper(3, 7);

	auto density = _eulerianSmokeData->densities()->datas();

	for (int i = lower.x; i < upper.x; ++i) {
		for (int j = lower.y; j < upper.y; ++j) {
			density(i, j) = 255;
		}
	}
}


void EulerianSmokeSolver2::addTemperature() {
	Vector2I lower(20, 0);
	Vector2I upper(26, 1);

	auto temperature = _eulerianSmokeData->temperature()->datas();

	for (int i = lower.x; i < upper.x; ++i) {
		for (int j = lower.y; j < upper.y; ++j) {
			temperature(i, j) = 30 + random_double(0, 1) * 10;
		}
	}
}



void EulerianSmokeSolver2::setMarkers() {
	auto size = resolution();
	cellCenterMarkers.reSize(size.x, size.y, FLUID);
}

void EulerianSmokeSolver2::setFluidCellNum() {
	auto vel = _eulerianSmokeData->velocity();

	vel->solveSystemMarker.reSize(resolution().x, resolution().y, -1);
	auto size = resolution();

	int nums = 0;
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			//如果格子被流体占据
			if (cellCenterMarkers(i, j) == FLUID) {
				vel->solveSystemMarker(i, j) = nums;
				nums += 1;
			}
		}
	}
}


//浮力总是沿着y方向的
void EulerianSmokeSolver2::accumulateBuoyancyForce(double timeIntervalInSeconds) {
	auto density = _eulerianSmokeData->densities();
	auto temperature = _eulerianSmokeData->temperature();
	auto flow = _eulerianSmokeData->velocity();

	auto vsize = flow->vSize();
	auto v = flow->vDatas();
	for (int i = 0; i < vsize.x; ++i) {
		for (int j = 0; j < vsize.y; ++j) {
			auto vposFunc = flow->vPosition();
			auto vpos = vposFunc(i, j);

			auto rho = density->sample(vpos);
			auto temper = temperature->sample(vpos);
			v(i, j) += (-smoke_alpha * rho + smoke_beta * (temper - amb_tempra)) * timeIntervalInSeconds;
		}
	}
}



EulerianSmokeSolver2::Builder EulerianSmokeSolver2::builder() {
	return Builder();
}

EulerianSmokeSolver2 EulerianSmokeSolver2::Builder::build() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return EulerianSmokeSolver2(_resolution, gridSpacing, _gridOrigin);
}


EulerianSmokeSolver2Ptr EulerianSmokeSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<EulerianSmokeSolver2>(
		new EulerianSmokeSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](EulerianSmokeSolver2* obj) {
			delete obj;
		});
}