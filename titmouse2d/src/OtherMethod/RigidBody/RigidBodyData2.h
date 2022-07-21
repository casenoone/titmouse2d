#pragma once

#include"../../Vector2.hpp"
#include "../../Array.hpp"
#include "../../Geometry/ExplicitSurface2.h"

class RigidBodyData2 {
public:
	RigidBodyData2() = default;
	RigidBodyData2(ExplicitSurface2Ptr rigidBodyList) {


		//��������λ��

	}
public:
	//����λ��
	Array<Vector2D>	vertex_positions;
	Array<Vector2D> vertex_velocities;
	Array<Vector2D>	vertex_forces;

	//����λ��
	Vector2D position;
	//�����ٶ�
	Vector2D velocity;
	//��������
	Vector2D force;
	//�����б�
	Array<ExplicitSurface2Ptr> rigidBodyList;
};

using RigidBodyData2Ptr = std::shared_ptr<RigidBodyData2>;

