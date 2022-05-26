#pragma once

#include "MassSpringData2.h"

class ConstrainedSolver2 {
public:
	ConstrainedSolver2(const std::vector<Vector2D>& pos) {
		massSpringData = std::make_shared<MassSpringData2>(pos);
	}


	void onAdvanceTimeStep(double dt);

private:
	//时间积分
	void timeIntegration(double dt);

	//构造Compliant Matrix
	void constructCompliantMat();

	//构造Jacobin Matrix
	void constructJacobinMat();

	//构造系统中质点之间的约束
	void constructConstraint();

	//求导数（用于组装jacobin中的元素）
	Vector2D getDerivation(int phi_idx, int x_idx);

	double computeConstraint(int idx);

	//构造解线性系统用到的constraint向量
	void construct_ConstraintVector(Eigen::VectorXd& vec);

	//构造求解线性系统用到的速度场向量组
	void construct_VelocityVector(Eigen::VectorXd& vec);

public:
	MassSpringData2Ptr massSpringData;
};