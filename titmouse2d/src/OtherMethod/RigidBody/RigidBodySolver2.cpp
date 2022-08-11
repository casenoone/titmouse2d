#include "RigidBodySolver2.h"
#include "../../MathUtils.hpp"

void RigidBodySolver2::computeExternalForce() {
	auto& vertex_force = rigidBodyData->vertex_forces;
	auto n = vertex_force.dataSize();

	static bool state = true;
	if (state) {
		vertex_force[2] = Vector2D(-1000, -100);
		vertex_force[0] = Vector2D(10, 100);

		state = false;
	}

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

	//更新质心位置
	Vector2D f(0, -2.5);

	//要去看一下数组求和的优化方法
	for (int i = 0; i < n; ++i) {
		f += forces[i];
	}

	vel += f * dt;
	pos += vel * dt;


	Vector3D temp_omega(0, 0, omega);
	Vector3D temp_torque(0, 0, torque);
	//更新角速度(记得写回omega)
	temp_omega += dt * I.inverse() * temp_torque;
	omega = temp_omega.z;
	//更新四元数
	//temp_omega.z = 10;

	//temp_q不要单位化
	Quaternion temp_q(0, 0.5 * dt * temp_omega);
	q = q + temp_q.cross(q);
	q.normalized();
	//更新旋转矩阵
	R = q.toMatrix();


	//修改r_i
	//这样的写法是错误的
	for (int i = 0; i < n; ++i) {
		//r[i] = R * r[i];
	}

	//更新顶点位置
	auto& vertexList = rigidBodyData->rigidBodyList->vertexList;
	for (int i = 0; i < n; ++i) {
		vertexList[i] = pos + R * r[i];
	}
}

void RigidBodySolver2::onAdvanceTimeStep(double dt) {
	clearForces();
	accumulateTotalForce(dt);
	computeTorque();
	computeInertia();
	timeIntegration(dt);
	CollisionResponseByImpulse();
}

//Array类的clear函数需要改进
void RigidBodySolver2::clearForces() {
	auto& force = rigidBodyData->vertex_forces;
	auto n = force.dataSize();
	force.clear();
	force.reSize(n);
}


void RigidBodySolver2::computeVertexVelocity() {
	auto& vertexList = rigidBodyData->rigidBodyList->vertexList;
	auto n = vertexList.size();
	auto& vel = rigidBodyData->velocity;
	auto& vertex_vel = rigidBodyData->vertex_velocities;
	auto& omega = rigidBodyData->palstance;
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;

	for (int i = 0; i < n; ++i) {
		Vector3D temp_omega(0, 0, omega);
		auto Rr = R * r[i];
		Vector3D temp_Rr(Rr, 0);
		auto temp_delta_v = temp_omega.cross(temp_Rr);
		auto delta_v = Vector2D(temp_delta_v.x, temp_delta_v.y);
		vertex_vel[i] = vel + delta_v;
	}
}


void RigidBodySolver2::CollisionResponseByImpulse() {
	auto& R = rigidBodyData->R;
	auto& r = rigidBodyData->r;
	auto& vertexList = rigidBodyData->rigidBodyList->vertexList;
	auto n = vertexList.size();
	auto& vel = rigidBodyData->velocity;
	auto& vertex_vel = rigidBodyData->vertex_velocities;
	auto& collder = rigidBodyData->collider;
	auto& I = rigidBodyData->inertia;
	auto I_inverse = I.inverse();
	auto& omega = rigidBodyData->palstance;
	Array<Vector2D> new_vertexVel(n);

	computeVertexVelocity();

	for (int i = 0; i < n; ++i) {
		auto state = collder->resolveCollision(0.02, 0.05, vertexList[i], vertex_vel[i], new_vertexVel[i]);
		if (state) {
			Vector3D temp_r(r[i], 0);
			auto Rr_mat = Matrix3x3<double>::constructMatrixProduct(R * temp_r);
			auto K = Matrix3x3<double>::identityMatrix() - Rr_mat * I_inverse * Rr_mat;
			auto j = K.inverse() * (new_vertexVel[i] - vertex_vel[i]);
			vel += j;

			Vector3D temp_j(j, 0);
			Vector3D temp_omega(0, 0, omega);
			temp_omega += I_inverse * ((R * temp_r).cross(temp_j));
			omega = temp_omega.z;
		}
	}


}