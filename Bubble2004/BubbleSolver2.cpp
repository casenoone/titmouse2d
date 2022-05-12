#include "bubbleSolver2.h"

static const double rad_k = 0.01;
BubbleSolver2::BubbleSolver2() {
	_particleSystemData = std::make_shared<BubbleData2>();
	_bubbleData = std::make_shared<BubbleData2>();
	_bubbleData = std::dynamic_pointer_cast<BubbleData2>(_particleSystemData);
}


void BubbleSolver2::emitParticles() {

}



Vector2D BubbleSolver2::computeF_rB(int i, int j) const {
	auto pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;

	auto p_ij = pos[i] - pos[j];
	auto rad_ij = radius[i] + radius[j];
	auto p_ij_norm2 = p_ij.getLength();

	return _bubbleData->kr * (1 / (p_ij_norm2)-(1 / rad_ij)) * p_ij;
}

Vector2D BubbleSolver2::computeF_aB(int i, int j) const {
	auto pos = _bubbleData->positions();
	auto& radius = _bubbleData->particleRadius;
	auto& neighbor = _bubbleData->neighbor->neighBors();

	int NB_i = 0;
	int NB_j = 0;

	auto& p_i = pos[i];
	auto& p_j = pos[j];
	auto p_ji = pos[j] - pos[i];
	auto p_ij_norm2 = (p_i - p_j).getLength();

	for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); iter++) {
		auto neighbor_index = *iter;
		auto rad_each = radius[i] + radius[neighbor_index];
		auto dis = pos[i].dis(pos[neighbor_index]);
		if (dis <= rad_each + rad_k && i != neighbor_index) {
			NB_i++;
		}
	}

	for (auto iter = neighbor[j].begin(); iter != neighbor[j].end(); iter++) {
		auto neighbor_index = *iter;
		auto rad_each = radius[j] + radius[neighbor_index];
		auto dis = pos[j].dis(pos[neighbor_index]);
		if (dis <= rad_each + rad_k && j != neighbor_index) {
			NB_j++;
		}
	}

	double inv_NBi = 0;
	double inv_NBj = 0;
	if (NB_i > 0)
		inv_NBi = 1 / NB_i;
	if (NB_j > 0)
		inv_NBj = 1 / NB_j;

	double c_nb = 0.5 * (inv_NBi + inv_NBj);
	double c_dist = (p_ij_norm2 - std::max(radius[i], radius[j]))
		/ std::min(radius[i], radius[j]);

	return _bubbleData->ka * c_nb * c_dist * (p_ji / p_ij_norm2);

}

void BubbleSolver2::computeF_a0() {

}

//暂时先不计算与固体的力
void BubbleSolver2::computeF_ra() {
	auto pos = _bubbleData->positions();
	auto n = _bubbleData->numberOfParticles();
	auto& forces = _bubbleData->forces();
	auto& neighbor = _bubbleData->neighbor->neighBors();
	auto& radius = _bubbleData->particleRadius;

	for (int i = 0; i < n; ++i) {
		Vector2D temp_f_r;
		Vector2D temp_f_a;
		for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); iter++) {
			auto neighbor_index = *iter;
			auto rad_each = radius[i] + radius[neighbor_index];
			auto dis = pos[i].dis(pos[neighbor_index]);
			if (dis <= rad_each + rad_k && i != neighbor_index) {
				temp_f_r += computeF_rB(i, neighbor_index);
				temp_f_a += computeF_aB(i, neighbor_index);
			}
		}

		forces[i] += (temp_f_r + temp_f_a);
		//std::cout << forces[i].x << std::endl;
	}
}

Vector2D BubbleSolver2::computeF_air(int i) {
	auto vel = _bubbleData->velocities();
	return -_bubbleData->kair * vel[i];
}

//这里暂时不累加fv还有与障碍物的力
void BubbleSolver2::computeF_fr() {
	auto n = _bubbleData->numberOfParticles();
	auto& forces = _bubbleData->forces();

	for (int i = 0; i < n; ++i) {
		forces[i] += computeF_air(i);
	}
}

void BubbleSolver2::computeTotalForce() {
	computeF_ra();
	computeF_fr();
}

void BubbleSolver2::bubbleBreakUp() {
	static int step = 0;
	if (_bubbleData->numberOfParticles() >= 2) {

		auto& pos = _bubbleData->positions();
		auto& vel = _bubbleData->velocities();
		auto& forces = _bubbleData->forces();
		auto& radius = _bubbleData->particleRadius;
		if (step % 100 == 0) {
			pos.pop_back();
			vel.pop_back();
			forces.pop_back();
			radius.pop_back();
			_newPositions.pop_back();
			_newVelocities.pop_back();
			_bubbleData->numberOfParticles()--;
		}
		step++;

	}
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


void BubbleSolver2::timeIntegration(double timeIntervalInSeconds) {

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

		newVelocity = vel[i] + (force[i] / 1) * timeIntervalInSeconds;
		auto& newPosition = _newPositions[i];
		auto temp2 = pos[i] + (newVelocity + induce_v) * timeIntervalInSeconds;
		newPosition = temp2;
	}


}

void BubbleSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {

	_bubbleData->neighbor->setNeiborList(0.12, _bubbleData->positions());

	beginAdvanceTimeStep();
	computeTotalForce();
	timeIntegration(timeIntervalInSeconds);
	//resolveCollision();
	endAdvanceTimeStep();
	bubbleBreakUp();
}