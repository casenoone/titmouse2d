#include "RigidBodySolver2.h"
#include "../../MathUtils.hpp"

void RigidBodySolver2::computeTorque() {
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;
	auto& forces = rigidBodyData->vertex_forces;
	auto& torque = rigidBodyData->torque;
	auto n = rigidBodyData->force.getLength();

	double tempT = 0.0;
	for (int i = 0; i < n; ++i) {
		tempT += (R * r[i]).cross(forces[i]);
	}
	torque = tempT;
}

void RigidBodySolver2::computeInertia() {
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;
	auto n = rigidBodyData->force.getLength();

	Matrix2x2<double> tempM(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < n; ++i) {

	}
}

void RigidBodySolver2::timeIntegration(double dt) {

}

void RigidBodySolver2::onAdvanceTimeStep(double dt) {

}