#pragma once

#include"../../Vector2.hpp"
#include "../../Array.hpp"
#include "../../Geometry/ExplicitSurface2.h"
#include "../../Matrix2x2.hpp"
#include "../../Matrix3x3.hpp"
#include "../../Quaternion.h"

//��ʱֻ��һ�����壬��������漰�����������֮�����ײ����ʱ�㲻��

class RigidBodyData2 {
public:
	RigidBodyData2() = default;
	RigidBodyData2(ExplicitSurface2Ptr rigidBodyList_) {
		//��������λ��,��ʱ����װ��
		auto& vertexList = rigidBodyList_->vertexList;
		auto size = vertexList.size();
		Vector2D tempP;
		for (auto i : vertexList) {
			tempP += i;
		}
		position = tempP / size;

		rigidBodyList = rigidBodyList_;
		vertex_forces.reSize(size);
		R = Matrix3x3<double>::identityMatrix();
	}
public:
	Array<Vector2D> vertex_velocities;
	Array<Vector2D>	vertex_forces;
	Array<Vector2D> r;

	//����λ��
	Vector2D position;
	//�����ٶ�
	Vector2D velocity;
	//��������
	Vector2D force;
	//��������
	double torque;
	//����ת������
	//Matrix2x2<double> inertia;
	Matrix3x3<double> inertia;
	//���Ľ��ٶȣ�������2D����⣬��˽��ٶ�Ϊ����
	double palstance = 0;
	//������̬����
	//Matrix2x2<double> R;
	Matrix3x3<double> R;
	Quaternion q;

	//�����б�(��ʱֻ��һ������)
	ExplicitSurface2Ptr rigidBodyList;
};

using RigidBodyData2Ptr = std::shared_ptr<RigidBodyData2>;

