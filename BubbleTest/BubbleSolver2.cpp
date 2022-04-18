#include "bubbleSolver2.h"

static const double stiffness = 2;
static const double drag = 5000;
BubbleSolver2::BubbleSolver2() {
	_particleSystemData = std::make_shared<BubbleData2>();
	_bubbleData = std::make_shared<BubbleData2>();
	_bubbleData = std::dynamic_pointer_cast<BubbleData2>(_particleSystemData);
}


void BubbleSolver2::emitParticles() {

}


Vector2D BubbleSolver2::computeSingeStrongForce(const Vector2D& p1,
	const Vector2D& p2,
	double l_ij) {
	Vector2D result;
	auto x_ij = p1 - p2.getNormalize();

	return -stiffness * (p1 - p2 - l_ij * x_ij);
}


void BubbleSolver2::computeDragForce(int i) {
	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();
	auto& n = _bubbleData->numberOfParticles();
	auto& vel = _bubbleData->velocities();

	force[i] += drag * radius[i] * radius[i] * -1 * vel(i);

}


void BubbleSolver2::computeDragForce() {
	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();
	auto& n = _bubbleData->numberOfParticles();
	auto& vel = _bubbleData->velocities();
	for (int i = 0; i < n; ++i) {
		force[i] += 1000 * radius[i] * radius[i] * -1 * vel(i);
	}


}


void BubbleSolver2::computeStrongForce() {
	auto n = _bubbleData->numberOfParticles();
	auto& neighbor = _bubbleData->neighbor->neighBors();
	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();



	auto& vel = _bubbleData->velocities();
	force.clear();

	//强相互作用力的计算
	for (int i = 0; i < n; ++i) {
		for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); iter++) {
			auto j = *iter;
			if (i != j) {
				auto r_i = radius[i];
				auto r_j = radius[j];
				auto rest_ij = computeRestLen(r_i, r_j);
				auto direction = pos[j] - pos[i];
				auto dis = pos[i].dis(pos[j]);
				if (dis > rest_ij && dis <= (r_i + r_j + 0.001)) {
					auto f = computeSingeStrongForce(pos[i], pos[j], rest_ij);
					force[i] += f;
				}


				else {
					computeDragForce(i);

				}
			}
		}
	}







}

void BubbleSolver2::timeIntegration(double timeIntervalInSeconds) {

	computeStrongForce();
	computeDragForce();
	int n = _bubbleData->numberOfParticles();
	auto& force = _bubbleData->forces();
	auto& pos = _bubbleData->positions();
	auto& vel = _bubbleData->velocities();

	for (int i = 0; i < n; ++i) {
		auto& newVelocity = _newVelocities[i];



		newVelocity = vel[i] + (force[i] / 1) * timeIntervalInSeconds;

		auto& newPosition = _newPositions[i];
		auto temp2 = pos[i] + newVelocity * timeIntervalInSeconds;
		newPosition = temp2;
	}

}

void BubbleSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	_bubbleData->neighbor->setNeiborList(0.12, _bubbleData->positions());
	beginAdvanceTimeStep();
	timeIntegration(timeIntervalInSeconds);
	//resolveCollision();
	endAdvanceTimeStep();
}