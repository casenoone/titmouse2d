#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"
#include "../titmouse2d/src/ConstVar.h"

static const double v_eps = 0.08;

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

	//以下是动力学求解部分
public:
	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	void emitVortexRing();

public:
	BubbleDataPtr _bubbleData;
};


inline Vector2D BubbleSolver2::computeUSingle(const Vector2D& pos, int i)const {

	auto& position = _bubbleData->vortexPosition;
	auto vorticity = _bubbleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (v_eps * v_eps)));

}