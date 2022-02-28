#include "VortexParticleSystemSolver2.h"

#include "../ConstVar.h"

#include "../random.h"

#include <iostream>

const double eps = 0.00001;

const Vector2D vs_vec = Vector2D(0.01, 0.0);

VortexParticleSystemSolver2::VortexParticleSystemSolver2() {
	_particleSystemData = make_shared<VortexParticleSystemData2>();
	_vortexParticleData = make_shared<VortexParticleSystemData2>();
	_vortexParticleData = std::dynamic_pointer_cast<VortexParticleSystemData2>(_particleSystemData);
}

VortexParticleSystemSolver2::~VortexParticleSystemSolver2() {

}

void VortexParticleSystemSolver2::timeIntegration(double timeIntervalInSeconds) {

	auto n = _vortexParticleData->numberOfParticles();
	auto positions = _vortexParticleData->positions();
	auto forces = _vortexParticleData->forces();

	//计算重力
	for (int i = 0; i < n; ++i) {
		//_newVelocities[i] += GRAVITY * timeIntervalInSeconds * -0.7;
		//cout << _newVelocities[0].x << endl;
	}


	for (int i = 0; i < n; ++i) {
		/*for (int j = 0; j < n; ++j) {
			if (i != j) {
				_newVelocities[i] += computeUSingle(positions[i], j);
			}
		}*/

		_newPositions[i] = positions[i] + timeIntervalInSeconds * _newVelocities[i];
	}


}

void VortexParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//onBeginAdvanceTimeStep();
	timeIntegration(timeIntervalInSeconds);
	//ParticleSystemSolver2::resolveCollision();
	onEndAdvanceTimeStep();
}


VortexParticleSystemData2Ptr& VortexParticleSystemSolver2::vortexParticleData() {
	return _vortexParticleData;
}


void VortexParticleSystemSolver2::onBeginAdvanceTimeStep() {
	ParticleSystemSolver2::beginAdvanceTimeStep();
}

void VortexParticleSystemSolver2::onEndAdvanceTimeStep() {
	ParticleSystemSolver2::endAdvanceTimeStep();
}


Vector2D VortexParticleSystemSolver2::computeUSingle(const Vector2D& pos, int i)const {

	auto position = _vortexParticleData->positions();
	auto vorticity = _vortexParticleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (eps * eps)));
}

void VortexParticleSystemSolver2::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_vortexParticleData->vorticities().reSize(numberOfParticles);

	auto vorticity = _vortexParticleData->vorticities();

	//为了方便测试，给每个粒子赋一个随机的涡量
	for (int i = 0; i < numberOfParticles; ++i) {
		//vorticity[i] = random_double(-0.01, 0.01);
		vorticity[i] = 0.000;
	}


}

void VortexParticleSystemSolver2::setPanels(ExplicitSurface2Ptr surfaces) {
	_vortexParticleData->panelSet = surfaces;
}

void VortexParticleSystemSolver2::emitParticlesFromPanel() {
	auto data = _vortexParticleData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto pos = data->positions();
	//遍历每个panel，在panel上随机生成粒子
	//还得建一个panel粒子的索引，便于边界处理时读取粒子的速度

	auto panelNum = panels->size();

	for (auto i = 0; i < panelNum; ++i) {
		const auto p = panels->lookAt(i);
		auto midPoint = 0.5 * (p.start + p.end);

		//在边界附近随机生成粒子

		auto line = (p.end - p.start).getNormalize();
		auto panelLength = (p.end - p.start).getLength();

		int emitNum = 3;
		for (int j = 0; j < emitNum; ++j) {

			auto lambda = random_double(0.0, panelLength);
			auto ppos = p.start + lambda * line;

			ppos += random_double(0.0, 0.03) * p.normal;

			pos.push(ppos);
			data->vorticities().push(0.0);
			_newVelocities.push(vs_vec);
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}

	}
}