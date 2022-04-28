#include "bubbleSolver2.h"

static const double stiffness = 3;
static const double drag = 1000;
BubbleSolver2::BubbleSolver2() {
	_particleSystemData = std::make_shared<BubbleData2>();
	_bubbleData = std::make_shared<BubbleData2>();
	_bubbleData = std::dynamic_pointer_cast<BubbleData2>(_particleSystemData);
}

void BubbleSolver2::emitVortexRing() {
	auto& pos = _bubbleData->vortexPosition;
	auto& vel = _bubbleData->vortexVelocity;
	auto& gamma = _bubbleData->vorticities();
	int n = 4;
	pos.reSize(n);
	vel.reSize(n);
	gamma.reSize(n);
	Vector2D A(0.4, 1.2);
	Vector2D B(0.4, 1.1);
	Vector2D C(0.4, 1.0);
	Vector2D D(0.4, 0.9);

	pos[0] = A;
	pos[1] = B;
	pos[2] = C;
	pos[3] = D;

	gamma[0] = 0.6;
	gamma[1] = 0.6;
	gamma[2] = -0.6;
	gamma[3] = -0.6;
}



Vector2D BubbleSolver2::computeSingeStrongForce(const Vector2D& p1,
	const Vector2D& p2,
	double l_ij) {
	Vector2D result;
	auto x_ij = p1 - p2.getNormalize();

	return -stiffness * (p1 - p2 - l_ij * x_ij);
}

void BubbleSolver2::computeRepulsiveForce() {

}

void BubbleSolver2::computeDragForce(const Voronoi2& voronoi, int i) {
	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();
	auto& n = _bubbleData->numberOfParticles();
	auto& vel = _bubbleData->velocities();

	force[i] += 2000 * radius[i] * radius[i] * -1 * vel(i);

}


void BubbleSolver2::computeDragForce(const Voronoi2& voronoi) {
	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();
	auto& n = _bubbleData->numberOfParticles();
	auto& vel = _bubbleData->velocities();
	for (int i = 0; i < n; ++i) {
		force[i] += drag * radius[i] * radius[i] * -1 * vel(i) * vel(i).getLength();
	}


}


void BubbleSolver2::computeStrongForce(const Voronoi2& voronoi) {
	//遍历维诺图的边，然后取出点的索引，计算两点之间的受力

	auto& pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& force = _bubbleData->forces();
	force.clear();
	auto& edges = voronoi._data.edges;
	auto len = edges.size();
	//强相互作用力的计算
	for (int k = 0; k < len; ++k) {
		auto i = voronoi._data.edges[k]->leftIndex;
		auto j = voronoi._data.edges[k]->rightIndex;
		auto r_i = radius[i];
		auto r_j = radius[j];
		auto rest_ij = computeRestLen(r_i, r_j);
		auto dis = pos[i].dis(pos[j]);
		if (dis > rest_ij && dis <= (r_i + r_j + 0.06)) {
			auto f = computeSingeStrongForce(pos[i], pos[j], rest_ij);
			force[i] += f;
			force[j] -= f;
		}
		else {
			computeDragForce(voronoi, i);

		}


	}
}

void BubbleSolver2::timeIntegration(double timeIntervalInSeconds, const Voronoi2& voronoi) {

	//更新涡粒子
	auto& vortex_pos = _bubbleData->vortexPosition;
	auto& vortex_vel = _bubbleData->vortexVelocity;
	auto vortex_num = vortex_pos.dataSize();

	Array<Vector2D> tempP(vortex_num);

	for (int i = 0; i < vortex_num; ++i) {
		Vector2D vortexVel;
		for (int j = 0; j < vortex_num; ++j) {
			if (i != j) {

				vortexVel += computeUSingle(vortex_pos[i], j);
			}
		}

		tempP[i] = vortex_pos[i] + timeIntervalInSeconds * vortexVel;
	}

	vortex_pos = tempP;

	computeStrongForce(voronoi);
	computeDragForce(voronoi);
	int n = _bubbleData->numberOfParticles();
	auto& force = _bubbleData->forces();
	auto& pos = _bubbleData->positions();
	auto& vel = _bubbleData->velocities();

	for (int i = 0; i < n; ++i) {
		auto& newVelocity = _newVelocities[i];


		//计算诱导速度
		/**********************/
		Vector2D induce_v;
		for (int j = 0; j < vortex_num; ++j) {
			induce_v += computeUSingle(pos[i], j);
		}

		/**********************/

		newVelocity = vel[i] + (force[i] / MASS) * timeIntervalInSeconds;

		//把涡粒子的诱导速度导致的位移加到这上面来
		_newPositions[i] = pos[i] + (newVelocity + induce_v) * timeIntervalInSeconds;
	}

}

void BubbleSolver2::onAdvanceTimeStep(double timeIntervalInSeconds, const Voronoi2& voronoi) {
	beginAdvanceTimeStep();
	timeIntegration(timeIntervalInSeconds, voronoi);
	//resolveCollision();
	endAdvanceTimeStep();
}