#include "ConstrainedSolver2.h"

//CompliantMat的尺寸：ExE
//E:约束的数量
//暂时使用稠密矩阵吧

//检查过了，完全没有问题
void ConstrainedSolver2::constructCompliantMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto c = 1 / massSpringData->stiff;
	auto& compliantMat = massSpringData->CompliantMat;
	compliantMat.resize(edgeNum, edgeNum);
	int j = 0;
	for (int i = 0; i < edgeNum; ++i) {
		for (int j = 0; j < edgeNum; ++j) {
			compliantMat(i, j) = 0;
			if (i == j) {
				compliantMat(i, j) = 0;
			}
		}

	}
}


//void ConstrainedSolver2::constructJacobinMat() {
//	auto& edge = massSpringData->edges;
//	auto edgeNum = edge.dataSize();
//	auto n = massSpringData->numberOfPoint;
//	auto& jacobinMat = massSpringData->JacobinMat;
//	jacobinMat.resize(edgeNum, 2 * n);
//
//	for (int i = 0; i < edgeNum; ++i) {
//		for (int j = 0; j < n * 2; j += 2) {
//			auto temp_v = getDerivation(i, j * 2);
//			jacobinMat(i, j) = temp_v.x;
//			jacobinMat(i, j + 1) = temp_v.y;
//		}
//	}
//
//}


//检查过了，没有问题
void ConstrainedSolver2::constructJacobinMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto n = massSpringData->numberOfPoint;
	auto& jacobinMat = massSpringData->JacobinMat;
	jacobinMat.resize(edgeNum, 2 * n);

	for (int i = 0; i < edgeNum; ++i) {
		for (int j = 0; j < n; ++j) {
			auto temp_v = getDerivation(i, j);
			jacobinMat(i, j) = temp_v.x;
			jacobinMat(i, j + n) = temp_v.y;
		}
	}

}

//检查过了，没有问题
void ConstrainedSolver2::constructConstraint() {
	auto& pos = massSpringData->positions;
	auto n = massSpringData->numberOfPoint;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	//这里要改进一下clear函数，有问题,
	edge.reSize(0);
	edge.clear();

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			auto& pos_i = pos[i];
			auto& pos_j = pos[j];
			//如果满足这个条件就建立约束
			if (pos_i.dis(pos_j) <= 0.08) {
				edge.push(MassSpringData2::Edge{ i,j });
			}
		}
	}
}


//检查过了，没有问题
Vector2D ConstrainedSolver2::getDerivation(int phi_idx, int x_idx) {
	auto& pos = massSpringData->positions;
	auto& edge = massSpringData->edges;

	int e_i = edge(phi_idx).i;
	int e_j = edge(phi_idx).j;

	if (e_i == x_idx) {
		auto vec_ei_ej = pos[e_i] - pos[e_j];
		return vec_ei_ej.getNormalize() * -1;
	}

	if (e_j == x_idx) {
		auto vec_ei_ej = pos[e_i] - pos[e_j];
		return vec_ei_ej.getNormalize() * 1;
	}
	return Vector2D::zero();
}

//检查过了，没有问题
double ConstrainedSolver2::computeConstraint(int idx) {
	auto& pos = massSpringData->positions;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	return (pos[edge[idx].i] - pos[edge[idx].j]).getLength() - restLen;
}

//检查过了，没有问题
void ConstrainedSolver2::construct_ConstraintVector(Eigen::VectorXd& vec) {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();

	for (int i = 0; i < edgeNum; ++i) {
		vec[i] = computeConstraint(i);
	}
}

//检察过了，没有问题
//千万记得要优化！优化！优化！
void ConstrainedSolver2::construct_VelocityVector(Eigen::VectorXd& vec) {
	auto& velocities = massSpringData->velocities;

	int n = massSpringData->numberOfPoint;
	for (int i = 0; i < n; ++i) {
		vec[i] = velocities[i].x;
		vec[i + n] = velocities[i].y;
	}

}

//检查过了，没有问题
void ConstrainedSolver2::onAdvanceTimeStep(double dt) {
	//构造质点之间的约束
	constructConstraint();

	//构造柔度矩阵
	constructCompliantMat();

	//构造雅可比矩阵
	constructJacobinMat();

	timeIntegration(dt);
}

//检查过了，没有问题
//如果想通过一点拉动整个系统，不要改变质点的速度，而是去修改质点的位置
//思考一下这是为什么
void ConstrainedSolver2::timeIntegration(double dt) {
	double t2 = dt * dt;
	auto& jacobinMat = massSpringData->JacobinMat;
	auto& compliantMat = massSpringData->CompliantMat;

	int n = massSpringData->numberOfPoint;
	int edgeNum = massSpringData->edges.dataSize();

	Eigen::VectorXd vel(2 * n, 1);
	Eigen::VectorXd phi(edgeNum, 1);
	Eigen::VectorXd lambda(edgeNum, 1);
	Eigen::VectorXd vel_new(2 * n, 1);

	construct_ConstraintVector(phi);
	construct_VelocityVector(vel);

	Eigen::MatrixXd	jacobin_trans = jacobinMat.transpose();
	auto A = (t2 * jacobinMat * jacobin_trans + compliantMat);

	auto b = -1 * phi - dt * jacobinMat * vel;
	lambda = A.colPivHouseholderQr().solve(b);
	vel_new = vel - dt * jacobin_trans * lambda;


	auto& velocities = massSpringData->velocities;
	auto& positions = massSpringData->positions;

	for (int i = 0; i < n; ++i) {
		velocities[i] = Vector2D(vel_new[i], vel_new[i + n]);
		//positions[i] += velocities[i] * dt;
	}

	for (int i = 0; i < n; ++i) {
		velocities[i] += (-1.0 * velocities[i]) * dt * 70;
	}

	//velocities[0] = Vector2D(0.5, 0);
	//velocities[1] = Vector2D(0.5, 0);
	for (int i = 0; i < n; ++i) {
		positions[i] += velocities[i] * dt;
	}
	positions[0] += dt * Vector2D(2, 0);
}