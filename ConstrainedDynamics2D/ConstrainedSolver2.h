#pragma once

#include "MassSpringData2.h"

class ConstrainedSolver2 {
public:
	ConstrainedSolver2(const std::vector<Vector2D>& pos) {
		massSpringData = std::make_shared<MassSpringData2>(pos);
	}

	//时间积分
	void timeIntegration(double dt);

	//构造Compliant Matrix
	void constructCompliantMat();

	//构造Jacobin Matrix
	void constructJacobinMat();

	//构造系统中质点之间的约束
	void constructConstraint();

	void onAdvanceTimeStep(double dt);


public:
	MassSpringData2Ptr massSpringData;
};