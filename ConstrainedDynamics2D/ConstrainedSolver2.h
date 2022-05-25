#pragma once

#include "MassSpringData2.h"

class ConstrainedSolver2 {
public:
	ConstrainedSolver2(const std::vector<Vector2D>& pos) {
		massSpringData = std::make_shared<MassSpringData2>(pos);
	}

	//ʱ�����
	void timeIntegration(double dt);

	//����Compliant Matrix
	void constructCompliantMat();

	//����Jacobin Matrix
	void constructJacobinMat();

	//����ϵͳ���ʵ�֮���Լ��
	void constructConstraint();

	void onAdvanceTimeStep(double dt);


public:
	MassSpringData2Ptr massSpringData;
};