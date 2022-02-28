#include "ApicSolver2.h"
#include "../boundingbox2.h"

ApicSolver2::ApicSolver2(
	const Vector2I& resolutions,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin)
	:PicSolver2(resolutions, gridSpacing, gridOrigin)
{
}


ApicSolver2::~ApicSolver2() {
}

void ApicSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	beginAdvanceTimeStep(timeIntervalInSeconds);
	computeExternalForces(timeIntervalInSeconds);
	computePressure(timeIntervalInSeconds);
	computeAdvection(timeIntervalInSeconds);
	endAdvanceTimeStep(timeIntervalInSeconds);
}



void ApicSolver2::transferFromParticlesToGrids() {
	auto flow = gridSystemData()->velocity();
	const auto particles = particleSystemData();
	const auto positions = particles->positions();
	auto velocities = particles->velocities();
	int numberOfParticles = particles->numberOfParticles();
	const auto hh = flow->gridSpacing() / 2.0;
	auto lower = flow->origin();
	Vector2D upper;
	upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
	upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;

	auto  bbox = BoundingBox2(lower, upper);

	_cX.reSize(numberOfParticles);
	_cY.reSize(numberOfParticles);

	//清空速度场
	flow->fill(Vector2D());

	auto u = flow->uDatas();
	auto v = flow->vDatas();

	const auto uPos = flow->uPosition();
	const auto vPos = flow->vPosition();

	Array2D uWeight;
	Array2D vWeight;
	uWeight.reSize(u.dataSize().x, u.dataSize().y);
	vWeight.reSize(v.dataSize().x, v.dataSize().y);

	_uMarkers.reSize(u.dataSize().x, u.dataSize().y, 0.0);
	_vMarkers.reSize(v.dataSize().x, v.dataSize().y, 0.0);


	LinearArraySampler2<double> uSampler(
		u,
		flow->gridSpacing(),
		flow->uOrigin()
	);

	LinearArraySampler2<double> vSampler(
		v,
		flow->gridSpacing(),
		flow->vOrigin()
	);

	for (int i = 0; i < numberOfParticles; ++i) {
		std::array<Vector2I, 4> indices;
		std::array<double, 4> weights;

		auto uPosClamped = positions.lookAt(i);
		uPosClamped.y = clamp(
			uPosClamped.y,
			bbox.lowerCorner.y + hh.y,
			bbox.upperCorner.y - hh.y);
		uSampler.getCoordinatesAndWeights(uPosClamped, &indices, &weights);

		for (int j = 0; j < 4; ++j) {
			auto gridPos = uPos(indices[j].x, indices[j].y);
			double apicTerm = _cX[i].dot(gridPos - uPosClamped);
			u(indices[j].x, indices[j].y) += weights[j] * (velocities[i].x + apicTerm);
			uWeight(indices[j].x, indices[j].y) += weights[j];
			_uMarkers(indices[j].x, indices[j].y) = 1;
		}


		auto vPosClamped = positions.lookAt(i);
		vPosClamped.x = clamp(
			vPosClamped.x,
			bbox.lowerCorner.x + hh.x,
			bbox.upperCorner.x - hh.x);
		vSampler.getCoordinatesAndWeights(vPosClamped, &indices, &weights);

		for (int j = 0; j < 4; ++j) {
			auto gridPos = vPos(indices[j].x, indices[j].y);
			double apicTerm = _cY[i].dot(gridPos - vPosClamped);
			v(indices[j].x, indices[j].y) += weights[j] * (velocities[i].y + apicTerm);
			vWeight(indices[j].x, indices[j].y) += weights[j];
			_vMarkers(indices[j].x, indices[j].y) = 1;
		}

	}

	uWeight.forEachIndex([&](int i, int j) {
		if (uWeight(i, j) > 0.0) {
			u(i, j) /= uWeight(i, j);
		}
		});
	vWeight.forEachIndex([&](int i, int j) {
		if (vWeight(i, j) > 0.0) {
			v(i, j) /= vWeight(i, j);
		}
		});

	setMarkers();
	setFluidCellNum();

}

void ApicSolver2::transferFromGridsToParticles() {
	auto flow = gridSystemData()->velocity();
	const auto particles = particleSystemData();
	const auto positions = particles->positions();
	auto velocities = particles->velocities();
	int numberOfParticles = particles->numberOfParticles();
	const auto hh = flow->gridSpacing() / 2.0;
	auto lower = flow->origin();
	Vector2D upper;
	upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
	upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;

	auto  bbox = BoundingBox2(lower, upper);

	_cX.reSize(numberOfParticles, Vector2D());
	_cY.reSize(numberOfParticles, Vector2D());

	auto u = flow->uDatas();
	auto v = flow->vDatas();

	LinearArraySampler2<double> uSampler(
		u,
		flow->gridSpacing(),
		flow->uOrigin()
	);

	LinearArraySampler2<double> vSampler(
		v,
		flow->gridSpacing(),
		flow->vOrigin()
	);


	for (int i = 0; i < numberOfParticles; ++i) {
		velocities[i] = flow->sample(positions.lookAt(i));

		std::array<Vector2I, 4> indices;
		std::array<Vector2D, 4> gradWeights;

		auto uPosClamped = positions.lookAt(i);
		uPosClamped.y = clamp(
			uPosClamped.y,
			bbox.lowerCorner.y + hh.y,
			bbox.upperCorner.y - hh.y);
		uSampler.getCoordinatesAndGradientWeights(
			uPosClamped, &indices, &gradWeights);
		for (int j = 0; j < 4; ++j) {
			_cX[i] += gradWeights[j] * u(indices[j].x, indices[j].y);
		}

		auto vPosClamped = positions.lookAt(i);
		vPosClamped.x = clamp(
			vPosClamped.x,
			bbox.lowerCorner.x + hh.x,
			bbox.upperCorner.x - hh.x);
		vSampler.getCoordinatesAndGradientWeights(
			vPosClamped, &indices, &gradWeights);
		for (int j = 0; j < 4; ++j) {
			_cY[i] += gradWeights[j] * v(indices[j].x, indices[j].y);
		}
	}
}



ApicSolver2::Builder ApicSolver2::builder() {
	return Builder();
}



ApicSolver2 ApicSolver2::Builder::build() const {
	auto gridspacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return ApicSolver2(_resolution, gridspacing, _gridOrigin);
}

ApicSolver2Ptr ApicSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<ApicSolver2>(
		new ApicSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](ApicSolver2* obj) {
			delete obj;
		});
}