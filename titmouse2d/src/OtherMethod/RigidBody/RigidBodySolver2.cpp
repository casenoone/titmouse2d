#include "RigidBodySolver2.h"
#include "../../MathUtils.hpp"

void RigidBodySolver2::computeExternalForce() {
	auto& vertex_force = rigidBodyData->vertex_forces;
	auto n = vertex_force.dataSize();
	vertex_force[2] = Vector2D(0.00000000000000000001, 0.00000000000000000001);
}

void RigidBodySolver2::computeTorque() {
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;
	auto& forces = rigidBodyData->vertex_forces;
	auto& torque = rigidBodyData->torque;
	auto n = rigidBodyData->rigidBodyList->vertexList.size();

	double tempT = 0.0;
	for (int i = 0; i < n; ++i) {
		tempT += (R * r[i]).cross(forces[i]);
	}

	torque = tempT;

}

void RigidBodySolver2::computeInertia() {
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;
	auto n = rigidBodyData->rigidBodyList->vertexList.size();
	auto& I = rigidBodyData->inertia;
	Matrix3x3<double> tempM;
	for (int i = 0; i < n; ++i) {
		Vector3D temp_r(r[i], 0);
		tempM += (temp_r.dot(temp_r) * Matrix3x3<double>::identityMatrix() - crossProduct(temp_r, temp_r));

	}
	I = R * tempM * R.transpose();
}

//ע������ÿһʱ��һ��Ҫ����
void RigidBodySolver2::accumulateGravityForce(double dt) {
	auto& vertex_force = rigidBodyData->vertex_forces;
	auto n = vertex_force.dataSize();
}

void RigidBodySolver2::accumulateTotalForce(double dt) {
	computeExternalForce();
}

void RigidBodySolver2::timeIntegration(double dt) {
	auto& torque = rigidBodyData->torque;
	auto& I = rigidBodyData->inertia;
	auto& r = rigidBodyData->r;
	auto& omega = rigidBodyData->palstance;
	auto& q = rigidBodyData->q;
	auto& R = rigidBodyData->R;
	auto& forces = rigidBodyData->vertex_forces;
	auto n = r.dataSize();
	auto& vel = rigidBodyData->velocity;
	auto& pos = rigidBodyData->position;

	//��������λ��
	Vector2D f(0, 0);

	//Ҫȥ��һ��������͵��Ż�����
	for (int i = 0; i < n; ++i) {
		f += forces[i];
	}

	vel += f * dt;
	pos += vel * dt;

	Vector3D temp_omega(0, 0, omega);
	Vector3D temp_torque(0, 0, torque);
	//���½��ٶ�(�ǵ�д��omega)
	temp_omega += dt * I.inverse() * temp_torque;
	omega = temp_omega.z;
	//std::cout << omega << std::endl;
	//������Ԫ��
	Quaternion temp_q(0, 0.5 * dt * temp_omega);
	temp_q.normalized();
	q = q + temp_q.cross(q);
	q.normalized();
	//������ת����
	R = q.toMatrix();

	//�޸�r_i
	for (int i = 0; i < n; ++i) {
		r[i] = R * r[i];
	}

	//���¶���λ��
	auto& vertexList = rigidBodyData->rigidBodyList->vertexList;
	for (int i = 0; i < n; ++i) {
		vertexList[i] = pos + r[i];
	}
}

void RigidBodySolver2::onAdvanceTimeStep(double dt) {
	clearForces();
	accumulateTotalForce(dt);
	computeTorque();
	computeInertia();
	timeIntegration(dt);
}

//Array���clear������Ҫ�Ľ�
void RigidBodySolver2::clearForces() {
	auto& force = rigidBodyData->vertex_forces;
	auto n = force.dataSize();
	force.clear();
	force.reSize(n);
}