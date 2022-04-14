#include "VortexParticleSystemSolver2.h"

#include "../ConstVar.h"

#include "../random.h"

#include "../Matrix3x3.hpp"

#include <iostream>
#include <cmath>
const double eps = 0.00001;

const Vector2D vs_vec = Vector2D(0.1, 0.0);

double vs_eps = 0.01;

//newPosition赋值给positions可以改用移动构造函数，提高效率

VortexParticleSystemSolver2::VortexParticleSystemSolver2() {
	_particleSystemData = std::make_shared<VortexParticleSystemData2>();
	_vortexParticleData = std::make_shared<VortexParticleSystemData2>();
	_vortexParticleData = std::dynamic_pointer_cast<VortexParticleSystemData2>(_particleSystemData);
}

VortexParticleSystemSolver2::~VortexParticleSystemSolver2() {

}

void VortexParticleSystemSolver2::timeIntegration(double timeIntervalInSeconds) {

	auto n = _vortexParticleData->numberOfParticles();
	auto positions = _vortexParticleData->positions();
	auto forces = _vortexParticleData->forces();


	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				_newVelocities[i] += computeUSingle(positions[i], j);
			}
		}

		_newPositions[i] = positions[i] + timeIntervalInSeconds * _newVelocities[i];
	}


}

void VortexParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//做vortex sheet boundary的时候要把这个onBeginAdvanceTimeStep注释掉
	onBeginAdvanceTimeStep();
	timeIntegration(timeIntervalInSeconds);
	ParticleSystemSolver2::resolveCollision();
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
		vorticity[i] = random_double(-0.01, 0.01);
		//vorticity[i] = 0.01;
	}


}



