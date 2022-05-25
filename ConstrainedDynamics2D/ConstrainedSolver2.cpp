#include "ConstrainedSolver2.h"

void ConstrainedSolver2::timeIntegration(double dt) {
	double t2 = dt * dt;
	auto& jacobinMat = massSpringData->JacobinMat;
	auto& compliantMat = massSpringData->CompliantMat;

	int n = massSpringData->numberOfPoint;
	int edgeNum = massSpringData->edges.dataSize();

	Eigen::VectorXd vel(2 * n, 1);
	Eigen::VectorXd phi(edgeNum, 1);

	construct_ConstraintVector(phi);
	construct_VelocityVector(vel);
}

//CompliantMat的尺寸：ExE
//E:约束的数量
//暂时使用稠密矩阵吧
void ConstrainedSolver2::constructCompliantMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto c = 1 / massSpringData->stiff;
	auto& compliantMat = massSpringData->CompliantMat;
	compliantMat.resize(edgeNum, edgeNum);
	int j = 0;
	for (int i = 0; i < edgeNum; ++i) {
		compliantMat(i, j++) = c;
	}
}


void ConstrainedSolver2::constructJacobinMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto n = massSpringData->numberOfPoint;
	auto& jacobinMat = massSpringData->JacobinMat;
	jacobinMat.resize(edgeNum, 2 * n);

	for (int i = 0; i < edgeNum; ++i) {
		for (int j = 0; j < n; j += 2) {
			auto temp_v = getDerivation(i, j * 2);
			jacobinMat(i, j) = temp_v.x;
			jacobinMat(i, j + 1) = temp_v.y;
		}
	}

}

void ConstrainedSolver2::constructConstraint() {
	auto& pos = massSpringData->positions;
	auto n = massSpringData->numberOfPoint;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			auto& pos_i = pos[i];
			auto& pos_j = pos[j];
			//如果满足这个条件就建立约束
			if (pos_i.dis(pos_j) <= restLen + 0.1) {
				edge.push(MassSpringData2::Edge{ i,j });
			}
		}
	}
}

Vector2D ConstrainedSolver2::getDerivation(int phi_idx, int x_idx) {
	auto& pos = massSpringData->positions;
	auto& edge = massSpringData->edges;

	int e_i = edge(phi_idx).i;
	int e_j = edge(phi_idx).j;

	if (e_i == x_idx) {
		auto vec_ei_ej = pos[e_i] - pos[e_j];
		return vec_ei_ej.getNormalize();
	}

	if (e_j == x_idx) {
		auto vec_ei_ej = pos[e_i] - pos[e_j];
		return vec_ei_ej.getNormalize() * -1;
	}
	return Vector2D::zero();
}


double ConstrainedSolver2::computeConstraint(int idx) {
	auto& pos = massSpringData->positions;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	return (pos[edge[idx].i] - pos[edge[idx].j]).getLength() - restLen;
}

void ConstrainedSolver2::construct_ConstraintVector(Eigen::VectorXd& vec) {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();

	for (int i = 0; i < edgeNum; ++i) {
		vec[i] = computeConstraint(i);
	}
}

//千万记得要优化！优化！优化！
void ConstrainedSolver2::construct_VelocityVector(Eigen::VectorXd& vec) {
	auto& velocities = massSpringData->velocities;

	int n = massSpringData->numberOfPoint;
	for (int i = 0; i < n; ++i) {
		vec[i] = velocities[i].x;
		vec[i + n] = velocities[i].y;
	}

}

void ConstrainedSolver2::onAdvanceTimeStep(double dt) {

}