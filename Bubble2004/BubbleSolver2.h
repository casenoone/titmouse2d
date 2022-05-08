#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"

class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	void timeIntegration(double timeIntervalInSeconds);

	void onAdvanceTimeStep(double timeIntervalInSeconds);

private:
	//计算bubble之间的排斥力
	Vector2D computeF_rB(int i, int j) const;

	//计算bubble之间的吸引力
	Vector2D computeF_aB(int i, int j) const;

	//计算障碍物对bubble的吸引力
	void computeF_a0();

	//计算所有的吸引力和排斥力
	void computeF_ra();

	//计算空气阻力
	Vector2D computeF_air(int i);

	//计算所有的阻力
	void computeF_fr();

	//计算所有的力
	void computeTotalForce();

	//气泡消失，随机删除气泡
	void bubbleBreakUp();

public:
	BubbleDataPtr _bubbleData;
};
