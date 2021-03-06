#pragma once

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/RegularPolygon.h"


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


class FoamVortexData : public ParticleSystemData2 {
public:
	FoamVortexData() = default;

	ArrayD& vorticities();

public:
	RegularPolygonPtr panelSet;

	//消去法向分量
	Eigen::VectorXd no_through_strength;

	//消去切向分量
	Eigen::VectorXd slip_Strength;

	//消去法向分量
	Eigen::MatrixXd no_through_matrix;

	//消去切向分量
	Eigen::MatrixXd slip_matrix;

	Array<Vector2D> newParticles;

	//tracer粒子的速度和位置
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

	//存放发射出去的涡粒子的信息
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

	//存放气泡对应的panel模型
	Array<RegularPolygonPtr> bubble_panelset;

	//气泡所需要用到的矩阵和gamma
	Eigen::MatrixXd bubble_slip_matrix;

	Array<Eigen::VectorXd> bubble_slip_strength;



private:

	//二维情况下，涡量是标量
	ArrayD _vorticities;


};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::vorticities() {
	return _vorticities;
}
