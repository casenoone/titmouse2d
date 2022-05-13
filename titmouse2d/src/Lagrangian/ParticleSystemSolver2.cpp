#include "ParticleSystemSolver2.h"
#include "../ConstVar.h"
#include "../random.h"

ParticleSystemSolver2::ParticleSystemSolver2() {
	_particleSystemData = std::make_shared<ParticleSystemData2>();
}

ParticleSystemSolver2::~ParticleSystemSolver2() {

}

void ParticleSystemSolver2::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {
	_particleSystemData->numberOfParticles() = numberOfParticles;
	_particleSystemData->positions() = pos;
	_particleSystemData->velocities().reSize(numberOfParticles);
	_particleSystemData->forces().reSize(numberOfParticles);


	////初始化neighborSearche
	_particleSystemData->neighbor = std::make_shared<NeighborSearcher2>(resolutionX, resolutionY, numberOfParticles);
}


void ParticleSystemSolver2::beginAdvanceTimeStep() {
	int n = _particleSystemData->numberOfParticles();

	_newPositions.clear();
	_newPositions.reSize(n);
	_newVelocities.clear();
	_newVelocities.reSize(n);

	clearForces(_particleSystemData->forces());
}


void ParticleSystemSolver2::endAdvanceTimeStep() {
	int n = _particleSystemData->numberOfParticles();
	auto& positions = _particleSystemData->positions();
	auto& velocities = _particleSystemData->velocities();



	positions.forEachIndex([&](int i) {
		positions[i] = _newPositions[i];
		});

	velocities.forEachIndex([&](int i) {
		velocities[i] = _newVelocities[i];
		});



}
void ParticleSystemSolver2::timeIntegration(double timeIntervalInSeconds) {
	int n = _particleSystemData->numberOfParticles();
	auto forces = _particleSystemData->forces();
	auto velocities = _particleSystemData->velocities();
	auto positions = _particleSystemData->positions();


	for (int i = 0; i < n; ++i) {

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
	int n = _particleSystemData->numberOfParticles();
	auto& velocities = _particleSystemData->velocities();
	if (_collider.IsNull() == false) {
		for (int i = 0; i < n; ++i) {
			_collider.resolveCollision(0.011 * random_double(0.0, 1.0), 0.00001, &_newPositions[i], &_newVelocities[i]);

		}
	}
}


void ParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	beginAdvanceTimeStep();
	accumulateForces(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	resolveCollision();
	endAdvanceTimeStep();

}

void ParticleSystemSolver2::setCollider(const Collider2& collider) {
	_collider = collider;
}



void ParticleSystemSolver2::accumulateForces(double timeIntervalInSeconds) {
	accumlateExternalForces();
}


void ParticleSystemSolver2::accumlateExternalForces() {
	//Gravity
	int n = _particleSystemData->numberOfParticles();
	auto& forces = _particleSystemData->forces();
	auto& velocities = _particleSystemData->velocities();
	auto& positions = _particleSystemData->positions();
	double mass = 1.0;
	auto g = Vector2D(0.0, -9.8);
	for (int i = 0; i < n; ++i) {
		auto force = g * mass;
		forces[i] = forces[i] + force;

		//Drag Forces
		forces[i] = forces[i] + velocities[i] * (-DRAG_COEFFICIENT);
	}



}


void ParticleSystemSolver2::clearForces(Array<Vector2D>& forces) {
	Vector2D zero;

	forces.forEachIndex([&](int i) {
		forces[i] = zero;
		});
}
