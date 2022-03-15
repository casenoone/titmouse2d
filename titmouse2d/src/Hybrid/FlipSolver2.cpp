#include "FlipSolver2.h"
#include "../MathUtils.hpp"

#include <omp.h>

FlipSolver2::FlipSolver2() :FlipSolver2({ 2,2 }, { 2,2 }, { 0,0 }) {

}


FlipSolver2::FlipSolver2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin)
	: PicSolver2(resolution, gridSpacing, gridOrigin) {

}

FlipSolver2::~FlipSolver2() {
}


double FlipSolver2::picBlendingFactor() const {
	return _picBlendingFactor;
}


//ע���������ϸ��
//ע��Ҫʹ��clamp����
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



	omp_set_num_threads(20);
#pragma omp parallel for
	for (int i = 0; i < sizeU.x; ++i) {
		for (int j = 0; j < sizeU.y; ++j) {
			_uDelta(i, j) = u(i, j);
		}
	}

	auto sizeV = vel->vSize();

	omp_set_num_threads(20);
#pragma omp parallel for
	for (int i = 0; i < sizeV.x; ++i) {
		for (int j = 0; j < sizeV.y; ++j) {
			_vDelta(i, j) = v(i, j);
		}
	}

}


void FlipSolver2::transferFromGridsToParticles() {

	auto flow = gridSystemData()->velocity();
	auto positions = particleSystemData()->positions();
	auto velocities = particleSystemData()->velocities();
	int numberOfParticles = particleSystemData()->numberOfParticles();

	//�����ٶ�����
	auto sizeU = flow->uSize();
	omp_set_num_threads(20);
#pragma omp parallel for
	for (int i = 0; i < sizeU.x; ++i) {
		for (int j = 0; j < sizeU.y; ++j) {
			_uDelta(i, j) = flow->u(i, j) - _uDelta(i, j);
		}
	}


	auto sizeV = flow->vSize();
	omp_set_num_threads(20);
#pragma omp parallel for
	for (int i = 0; i < sizeV.x; ++i) {
		for (int j = 0; j < sizeV.y; ++j) {
			_vDelta(i, j) = flow->v(i, j) - _vDelta(i, j);
		}
	}


	//ÿһ��ʱ������ȥ����һ��sampler���ܷ�ֻ����һ�飬����Ч�ʸ���
	LinearArraySampler2<double> uSampler(_uDelta, flow->gridSpacing(), flow->origin());
	LinearArraySampler2<double> vSampler(_vDelta, flow->gridSpacing(), flow->origin());

	//��flip��deltaֵӳ���particles
	omp_set_num_threads(20);
#pragma omp parallel for
	for (int i = 0; i < numberOfParticles; ++i) {
		auto sampleDelta = Vector2D(uSampler(_uDelta, positions[i]), vSampler(_vDelta, positions[i]));
		Vector2D flipVel = velocities[i] + sampleDelta;

		if (_picBlendingFactor > 0.0) {
			Vector2D picVel = flow->sample(positions[i]);


			flipVel = lerp(flipVel, picVel, _picBlendingFactor);
		}
		velocities[i] = flipVel;

	}
}


FlipSolver2::Builder FlipSolver2::builder() {
	return Builder();
}



FlipSolver2 FlipSolver2::Builder::build() const {
	auto gridspacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return FlipSolver2(_resolution, gridspacing, _gridOrigin);
}

FlipSolver2Ptr FlipSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<FlipSolver2>(
		new FlipSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](FlipSolver2* obj) {
			delete obj;
		});
}