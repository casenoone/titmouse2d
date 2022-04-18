#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"
#include <Eigen/Dense>
#include "../titmouse2d/src/Geometry/RegularPolygon.h"


class BubbleData2 : public ParticleSystemData2 {
public:
	BubbleData2() = default;

	class Panel {
	public:
		Panel() :
			start(Vector2D::zero()),
			end(Vector2D::zero()),
			mid(Vector2D::zero()),
			normal(Vector2D::zero()),
			tangent(Vector2D::zero()),
			gamma(0.0)
		{}


	public:
		Vector2D start;
		Vector2D end;
		Vector2D mid;
		Vector2D normal;
		Vector2D tangent;
		double gamma;
	};

	ArrayD& vorticities();


public:
	ArrayD particleRadius;

	RegularPolygonPtr panelSet;

	//消去法向分量
	Eigen::VectorXd strength;

	//消去切向分量
	Eigen::VectorXd slipStrength;

	//消去法向分量
	Eigen::MatrixXd A;

	//消去切向分量
	Eigen::MatrixXd B;

	Array<Vector2D> newParticles;

	//存放发射出去的涡粒子的信息
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

	//tracer粒子的速度和位置
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

private:
	//二维情况下，涡量是标量
	ArrayD _vorticities;
};

using BubbleDataPtr = std::shared_ptr<BubbleData2>;

inline ArrayD& BubbleData2::vorticities() {
	return _vorticities;
}