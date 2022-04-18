#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"

//调节气泡的干湿状态
static const double lambda = 0.0;

class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	void timeIntegration(double timeIntervalInSeconds);

	void onAdvanceTimeStep(double timeIntervalInSeconds);

	double computeRestLen(double r_i, double r_j);

	void computeStrongForce();

	void computeDragForce(int i);

	void computeDragForce();


	Vector2D computeSingeStrongForce(const Vector2D& p1, const Vector2D& p2, double l_ij);

public:
	BubbleDataPtr _bubbleData;
};


inline double BubbleSolver2::computeRestLen(double r_i, double r_j) {

	return std::sqrt(r_i * r_i + r_j * r_j + (3 * lambda - 1) * r_i * r_j);
	return r_i + r_j;
}