#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"
#include "../Voronoi2D/Voronoi2.h"

class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	void timeIntegration(double timeIntervalInSeconds, const Voronoi2& voronoi);

	void onAdvanceTimeStep(double timeIntervalInSeconds, const Voronoi2& voronoi);

public:
	BubbleDataPtr _bubbleData;
};