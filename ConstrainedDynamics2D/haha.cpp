#include "ConstrainedSolver2.h"
#include<Eigen/IterativeLinearSolvers>

//CompliantMat�ĳߴ磺ExE
//E:Լ��������

void ConstrainedSolver2::constructCompliantMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto c = 1 / massSpringData->stiff;
	auto& compliantMat = massSpringData->CompliantMat;
	compliantMat.resize(edgeNum, edgeNum);
	int j = 0;
	for (int i = 0; i < edgeNum; ++i) {
		compliantMat.insert(i, j++) = c;
	}
}


void ConstrainedSolver2::constructJacobinMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto n = massSpringData->numberOfPoint;
	auto& jacobinMat = massSpringData->JacobinMat;
	jacobinMat.resize(edgeNum, 2 * n);

	for (int i = 0; i < edgeNum; ++i) {
		for (int j = 0; j < n; ++j) {
			auto temp_v = getDerivation(i, j);
			if (temp_v.x)
				jacobinMat.insert(i, j) = temp_v.x;
			if (temp_v.y)
				jacobinMat.insert(i, j + n) = temp_v.y;
		}
	}
}

void ConstrainedSolver2::constructConstraint() {
	auto& pos = massSpringData->positions;
	auto n = massSpringData->numberOfPoint;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	//����Ҫ�Ľ�һ��clear������������,
	edge.reSize(0);
	edge.clear();
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			auto& pos_i = pos[i];
			auto& pos_j = pos[j];
			//���������������ͽ���Լ��

			//�����2023�ر�棬�ǵøû�ȥ


			if (pos_i.dis(pos_j) <= 0.05 && pos_i.dis(pos_j) >= 0.04) {
				edge.push(MassSpringData2::Edge{ i,j });
			}
		}
	}



}

//ע�����������ķ���һ����Ҫ�����
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

void ConstrainedSolver2::construct_VelocityVector(Eigen::VectorXd& vec) {
	auto& velocities = massSpringData->velocities;

	int n = massSpringData->numberOfPoint;
	for (int i = 0; i < n; ++i) {
		vec[i] = velocities[i].x;
		vec[i + n] = velocities[i].y - 0.5 * 0.01;
	}

}

//���㵯��֮�������
Vector2D ConstrainedSolver2::computeSpringDragForce(int idx) {
	auto& pos = massSpringData->positions;
	auto& vel = massSpringData->velocities;
	auto& edges = massSpringData->edges;
	double& dc = massSpringData->dampingCoeff;

	int i = edges[idx].i;
	int j = edges[idx].j;

	auto dir = (pos[i] - pos[j]).getNormalize();

	auto damping = -dc * (dir.dot(vel[i] - vel[j])) * dir;
	return damping;
}


void ConstrainedSolver2::onAdvanceTimeStep(double dt) {


	//������Ⱦ���
	constructCompliantMat();

	//�����ſɱȾ���
	constructJacobinMat();

	//ʱ�����
	timeIntegration(dt);
}


void ConstrainedSolver2::timeIntegration(double dt) {
	double t2 = dt * dt;
	auto& jacobinMat = massSpringData->JacobinMat;
	auto& compliantMat = massSpringData->CompliantMat;

	int n = massSpringData->numberOfPoint;
	auto& edges = massSpringData->edges;
	int edgeNum = massSpringData->edges.dataSize();
	auto& pos = massSpringData->positions;

	Eigen::VectorXd vel(2 * n, 1);
	Eigen::VectorXd phi(edgeNum, 1);
	Eigen::VectorXd lambda(edgeNum, 1);
	Eigen::VectorXd vel_new(2 * n, 1);

	auto& velocities = massSpringData->velocities;
	auto& positions = massSpringData->positions;

	construct_ConstraintVector(phi);
	construct_VelocityVector(vel);

	auto jacobin_trans = jacobinMat.transpose();
	auto A = (t2 * jacobinMat * jacobin_trans + compliantMat);

	auto b = -1 * phi - dt * jacobinMat * vel;
	Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower | Eigen::Upper> cg;
	cg.compute(A);
	lambda = cg.solve(b);

	//ע������ķ��ţ���Ƶ��д����
	vel_new = vel + dt * jacobin_trans * lambda;

	for (int i = 0; i < n; ++i) {
		velocities[i] = Vector2D(vel_new[i], vel_new[i + n]);
	}

	for (int k = 0; k < edgeNum; ++k) {
		int i = edges[k].i;
		int j = edges[k].j;
		auto damping = computeSpringDragForce(k) * 0;
		velocities[i] += damping;
		velocities[j] -= damping;
	}

	//����
	for (int i = 0; i < n; ++i) {
		//velocities[i] += (-1.0 * velocities[i]) * dt * 0.5;
	}

	//λ�ø���
	for (int i = 0; i < n; ++i) {
		positions[i] += velocities[i] * dt;
	}

	//�߽紦��
	for (int i = 0; i < n; ++i) {
		if (positions[i].x < 0)
		{
			positions[i].x = 0;
			velocities[i].x = 0;
		}
		if (positions[i].x > 2)
		{
			positions[i].x = 2;
			velocities[i].x = 0;
		}
		if (positions[i].y < 0)
		{
			positions[i].y = 0;
			velocities[i].y = 0;
		}
		if (positions[i].y > 2)
		{
			positions[i].y = 2;
			velocities[i].y = 0;
		}
	}
}