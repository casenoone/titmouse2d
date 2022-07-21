#pragma once

#include "RigidBodyData2.h"

class RigidBodySolver2 {
public:
	RigidBodySolver2() = default;

	RigidBodySolver2(ExplicitSurface2Ptr rigidBodyList) {
		rigidBodyData = std::make_shared<RigidBodyData2>(rigidBodyList);
	}

	void timeIntegration(double dt);

	void onAdvanceTimeStep(double dt);

public:
	RigidBodyData2Ptr rigidBodyData;
};

using RigidBodySolver2Ptr = std::shared_ptr<RigidBodySolver2>;
