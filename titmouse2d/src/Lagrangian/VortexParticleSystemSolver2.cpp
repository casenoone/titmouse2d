#include "VortexParticleSystemSolver2.h"

#include "../ConstVar.h"

#include "../random.h"

const double eps = 0.00001;

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
		_newVelocities[i] += GRAVITY * timeIntervalInSeconds * 0.7;
	}


	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				_newVelocities[i] += computeUSingle(positions[i], j);
			}
		}

		_newPositions[i] += positions[i] + timeIntervalInSeconds * _newVelocities[i];
	}


}

void VortexParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
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


Vector2<double> VortexParticleSystemSolver2::computeUSingle(const Vector2<double>& pos, int i)const {

	auto position = _vortexParticleData->positions();
	auto vorticity = _vortexParticleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2<double>(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (eps * eps)));
}

void VortexParticleSystemSolver2::setData(int numberOfParticles,
	ArrayPtr<Vector2<double>>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_vortexParticleData->vorticities().reSize(numberOfParticles);

	auto vorticity = _vortexParticleData->vorticities();

	//为了方便测试，给每个粒子赋一个随机的涡量
	for (int i = 0; i < numberOfParticles; ++i) {
		vorticity[i] = random_double(-0.01, 0.01);
		//vorticity[i] = 0.001;
	}
}
