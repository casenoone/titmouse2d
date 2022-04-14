#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"

class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	virtual void timeIntegration(double timeIntervalInSeconds)override;

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

public:
	BubbleDataPtr _bubbleData;

};