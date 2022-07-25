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
	auto& I = rigidBodyData->inertia;

	Matrix3x3<double> tempM;
	for (int i = 0; i < n; ++i) {
		Vector3D temp_r(r[i], 0);
		tempM += (temp_r.dot(temp_r) * Matrix3x3<double>::identityMatrix() - crossProduct(temp_r, temp_r));

	}
	I = R * tempM * R.inverse();
}

void RigidBodySolver2::timeIntegration(double dt) {
	auto& torque = rigidBodyData->torque;
	auto& I = rigidBodyData->inertia;
	auto& r = rigidBodyData->r;
	auto& omega = rigidBodyData->palstance;

	Vector3D temp_omega(0, 0, omega);
	Vector3D temp_torque(0, 0, torque);
	//更新角速度(记得写回omega)
	temp_omega += dt * I.inverse() * temp_torque;

	//更新四元数

	//更新速度

	//更新质心位置

	//更新顶点位置
}

void RigidBodySolver2::onAdvanceTimeStep(double dt) {

}