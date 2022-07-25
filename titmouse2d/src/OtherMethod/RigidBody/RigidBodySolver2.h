#pragma once

#include "RigidBodyData2.h"

class RigidBodySolver2 {
public:
	RigidBodySolver2() = default;

	RigidBodySolver2(ExplicitSurface2Ptr rigidBodyList) {
		rigidBodyData = std::make_shared<RigidBodyData2>(rigidBodyList);
		computeReference();
	}

	void computeExternalForce();

	void computeTorque();

	void computeInertia();

	void accumulateGravityForce(double dt);

	void accumulateTotalForce(double dt);

	void timeIntegration(double dt);

	void onAdvanceTimeStep(double dt);

	void clearForces();

private:
	//只计算一次
	void computeReference() {
		int n = rigidBodyData->rigidBodyList->vertexList.size();
		auto& r = rigidBodyData->r;
		r.reSize(n);
		auto& vertexList = rigidBodyData->rigidBodyList->vertexList;
		for (int i = 0; i < n; ++i) {
			r[i] = vertexList[i] - rigidBodyData->position;
		}
	}

public:
	RigidBodyData2Ptr rigidBodyData;
};

using RigidBodySolver2Ptr = std::shared_ptr<RigidBodySolver2>;
