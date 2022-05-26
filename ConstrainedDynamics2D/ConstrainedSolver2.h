#pragma once

#include "MassSpringData2.h"

class ConstrainedSolver2 {
public:
	ConstrainedSolver2(const std::vector<Vector2D>& pos) {
		massSpringData = std::make_shared<MassSpringData2>(pos);
	}


	void onAdvanceTimeStep(double dt);

private:
	//ʱ�����
	void timeIntegration(double dt);

	//����Compliant Matrix
	void constructCompliantMat();

	//����Jacobin Matrix
	void constructJacobinMat();

	//����ϵͳ���ʵ�֮���Լ��
	void constructConstraint();

	//������������װjacobin�е�Ԫ�أ�
	Vector2D getDerivation(int phi_idx, int x_idx);

	double computeConstraint(int idx);

	//���������ϵͳ�õ���constraint����
	void construct_ConstraintVector(Eigen::VectorXd& vec);

	//�����������ϵͳ�õ����ٶȳ�������
	void construct_VelocityVector(Eigen::VectorXd& vec);

public:
	MassSpringData2Ptr massSpringData;
};