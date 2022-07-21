#pragma once

#include"../../Vector2.hpp"
#include "../../Array.hpp"
#include "../../Geometry/ExplicitSurface2.h"

class RigidBodyData2 {
public:
	RigidBodyData2() = default;
	RigidBodyData2(ExplicitSurface2Ptr rigidBodyList) {


		//计算质心位置

	}
public:
	//顶点位置
	Array<Vector2D>	vertex_positions;
	Array<Vector2D> vertex_velocities;
	Array<Vector2D>	vertex_forces;

	//质心位置
	Vector2D position;
	//质心速度
	Vector2D velocity;
	//质心受力
	Vector2D force;
	//刚体列表
	Array<ExplicitSurface2Ptr> rigidBodyList;
};

using RigidBodyData2Ptr = std::shared_ptr<RigidBodyData2>;

