#include "ParticleSystemSolver2.h"
#include "../ConstVar.h"


ParticleSystemSolver2::ParticleSystemSolver2() {
	_particleSystemData = make_shared<ParticleSystemData2>();
}

ParticleSystemSolver2::~ParticleSystemSolver2() {

}

void ParticleSystemSolver2::setData(size_t numberOfParticles, Array<Vector2<double>>& pos, size_t resolutionX, size_t resolutionY) {
	_particleSystemData->numberOfParticles() = numberOfParticles;
	_particleSystemData->positions() = pos;
	_particleSystemData->velocities().reSize(numberOfParticles);
	_particleSystemData->forces().reSize(numberOfParticles);
	
	
	//初始化neighborSearche
	_particleSystemData->neighbor = make_shared<NeighborSearcher2>(resolutionX, resolutionY, numberOfParticles);
	_particleSystemData->neighbor->setNeiborList(pos);
}


void ParticleSystemSolver2::beginAdvanceTimeStep() {
	size_t n = _particleSystemData->numberOfParticles();
	_newPositions.reSize(n);
	_newVelocities.reSize(n);

	clearForces(_particleSystemData->forces());
}


void ParticleSystemSolver2::endAdvanceTimeStep() {
	size_t n = _particleSystemData->numberOfParticles();
	auto& positions = _particleSystemData->positions();
	auto& velocities = _particleSystemData->velocities();



	positions.forEachIndex([&](size_t i) {
		positions[i] = _newPositions[i];
		});

	velocities.forEachIndex([&](size_t i) {
		velocities[i] = _newVelocities[i];
		});

}
void ParticleSystemSolver2::timeIntegration(double timeIntervalInSeconds) {
	size_t n = _particleSystemData->numberOfParticles();
	auto forces = _particleSystemData->forces();
	auto velocities = _particleSystemData->velocities();
	auto positions = _particleSystemData->positions();


	for (size_t i = 0; i < n; ++i) {
		auto& newVelocity = _newVelocities[i];
	
		auto temp1 = velocities[i] + (forces[i] / MASS) * timeIntervalInSeconds;
		newVelocity = temp1;

		auto& newPosition = _newPositions[i];
		auto temp2 = positions[i] + newVelocity * timeIntervalInSeconds;
		newPosition = temp2;
	}

}

//暂时不实现
void ParticleSystemSolver2::resolveCollision() {
	
	
}


void ParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	beginAdvanceTimeStep();
	accumulateForces(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	resolveCollision();
	endAdvanceTimeStep();
}


void ParticleSystemSolver2::accumulateForces(double timeIntervalInSeconds) {
	accumlateExternalForces();
}


void ParticleSystemSolver2::accumlateExternalForces() {
	//Gravity
	size_t n = _particleSystemData->numberOfParticles();
	auto &forces = _particleSystemData->forces();
	auto &velocities = _particleSystemData->velocities();
	auto &positions = _particleSystemData->positions();
	double mass = 1.0;

	for (size_t i = 0; i < n; ++i) {
		auto force = GRAVITY * mass;
		forces[i] = forces[i] + force;

		//Drag Forces
		forces[i] = forces[i] + velocities[i] * (-DRAG_COEFFICIENT);
	}



}


void ParticleSystemSolver2::clearForces(Array<Vector2<double>>& forces) {
	Vector2<double> zero;
	
	forces.forEachIndex([&](size_t i) {
		forces[i] = zero;
	});
}