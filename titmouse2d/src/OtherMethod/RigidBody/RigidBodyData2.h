#pragma once

#include"../../Vector2.hpp"
#include "../../Array.hpp"
#include "../../Geometry/ExplicitSurface2.h"
#include "../../Matrix2x2.hpp"
#include "../../Matrix3x3.hpp"
#include "../../Quaternion.h"

//暂时只搞一个刚体，多个刚体涉及到刚体与刚体之间的碰撞，暂时搞不定

class RigidBodyData2 {
public:
	RigidBodyData2() = default;
	RigidBodyData2(ExplicitSurface2Ptr rigidBodyList_) {
		//计算质心位置,暂时不封装了
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

	//质心位置
	Vector2D position;
	//质心速度
	Vector2D velocity;
	//质心受力
	Vector2D force;
	//质心力矩
	double torque;
	//质心转动惯量
	//Matrix2x2<double> inertia;
	Matrix3x3<double> inertia;
	//质心角速度，由于是2D下求解，因此角速度为标量
	double palstance = 0;
	//刚体姿态矩阵
	//Matrix2x2<double> R;
	Matrix3x3<double> R;
	Quaternion q;

	//刚体列表(暂时只搞一个刚体)
	ExplicitSurface2Ptr rigidBodyList;
};

using RigidBodyData2Ptr = std::shared_ptr<RigidBodyData2>;

