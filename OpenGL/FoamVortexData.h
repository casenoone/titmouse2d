#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/RegularPolygon.h"

#include "MovingFaceCenteredGrid2.h"

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

	ArrayD& gamma();

public:
	RegularPolygonPtr panelSet;

	//2021 paper 边界处理
	//消去切向分量
	Eigen::VectorXd slip_strength;

	//消去法向分量
	Eigen::VectorXd no_throught_strength;

	//消去法向分量
	Eigen::MatrixXd A;

	//消去切向分量
	Eigen::MatrixXd slip_matrix;

	//保存发射粒子的位置
	Array<Vector2D> newParticles;

	//tracer粒子的速度和位置
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

	//存放发射出去的涡粒子的信息
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

private:

	//二维情况下，涡量是标量
	ArrayD _gamma;


};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::gamma() {
	return _gamma;
}

#endif
