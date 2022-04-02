#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/ExplicitSurface2.h"

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

	ArrayD& vorticities();

public:
	ExplicitSurface2Ptr panelSet;

	MovingFaceCenteredGrid2Ptr movingGrid;

	//Array<Panel> panelSet;

	//消去法向分量
	Eigen::VectorXd strength;

	//消去切向分量
	Eigen::VectorXd slipStrength;

	//消去法向分量
	Eigen::MatrixXd A;

	//消去切向分量
	Eigen::MatrixXd B;

	Array<Vector2D> newParticles;

	//tracer粒子的速度和位置
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

private:

	//二维情况下，涡量是标量
	ArrayD _vorticities;


};

using FoamVortexDataPtr = shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::vorticities() {
	return _vorticities;
}

#endif
