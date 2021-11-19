#include "FlipSolver2.h"
#include "../MathUtils.hpp"

FlipSolver2::FlipSolver2() :FlipSolver2({ 2,2 }, { 2,2 }, { 0,0 }) {

}


FlipSolver2::FlipSolver2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin)
	: PicSolver2(resolution, gridSpacing, gridOrigin) {

}

FlipSolver2::~FlipSolver2() {
}


double FlipSolver2::picBlendingFactor() const {
	return _picBlendingFactor;
}


//注意这种设计细节
//注意要使用clamp函数
void FlipSolver2::setPicBlendingFactor(double factor) {
	_picBlendingFactor = clamp(factor, 0.0, 1.0);
}


void FlipSolver2::transferFromParticlesToGrids() {
	PicSolver2::transferFromParticlesToGrids();

	auto vel = gridSystemData()->velocity();
	auto u = vel->uDatas();
	auto v = vel->vDatas();

	_uDelta.reSize(u.dataSize().x, u.dataSize().y, 0.0);
	_vDelta.reSize(v.dataSize().x, v.dataSize().y, 0.0);

	auto sizeU = vel->uSize();
	for (size_t i = 0; i < sizeU.x; ++i) {
		for (size_t j = 0; j < sizeU.y; ++j) {
			_uDelta(i, j) = u(i, j);
		}
	}

	auto sizeV = vel->vSize();
	for (size_t i = 0; i < sizeV.x; ++i) {
		for (size_t j = 0; j < sizeV.y; ++j) {
			_vDelta(i, j) = v(i, j);
		}
	}


}


void FlipSolver2::transferFromGridsToParticles() {

	auto flow = gridSystemData()->velocity();
	auto positions = particleSystemData()->positions();
	auto velocities = particleSystemData()->velocities();
	size_t numberOfParticles = particleSystemData()->numberOfParticles();

	//计算速度增量
	auto sizeU = flow->uSize();
	for (size_t i = 0; i < sizeU.x; ++i) {
		for (size_t j = 0; j < sizeU.y; ++j) {
			_uDelta(i, j) = flow->u(i, j) - _uDelta(i, j);
		}
	}


	auto sizeV = flow->vSize();
	for (size_t i = 0; i < sizeV.x; ++i) {
		for (size_t j = 0; j < sizeV.y; ++j) {
			_vDelta(i, j) = flow->v(i, j) - _vDelta(i, j);
		}
	}

	LinearArraySampler2<double> uSampler(_uDelta, flow->gridSpacing(), flow->origin());
	LinearArraySampler2<double> vSampler(_vDelta, flow->gridSpacing(), flow->origin());

	//把flip的delta值映射回particles
	for (size_t i = 0; i < numberOfParticles; ++i) {
		auto sampleDelta = Vector2<double>(uSampler(_uDelta, positions[i]), vSampler(_vDelta, positions[i]));
		Vector2<double> flipVel = velocities[i] + sampleDelta;

		if (_picBlendingFactor > 0.0) {
			Vector2<double> picVel = flow->sample(positions[i]);

			
			flipVel = lerp(flipVel, picVel, _picBlendingFactor);
		}
		velocities[i] = flipVel;

	}

}


FlipSolver2::Builder FlipSolver2::builder() {
	return Builder();
}



FlipSolver2 FlipSolver2::Builder::build() const {
	auto gridspacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return FlipSolver2(_resolution, gridspacing, _gridOrigin);
}

FlipSolver2Ptr FlipSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<FlipSolver2>(
		new FlipSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](FlipSolver2* obj) {
			delete obj;
		});
}