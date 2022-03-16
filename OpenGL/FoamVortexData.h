#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/ExplicitSurface2.h"

#include "MovingFaceCenteredGrid2.h"

class FoamVortexData : public ParticleSystemData2 {
public:
	FoamVortexData() = default;

	ArrayD& vorticities();

public:
	ExplicitSurface2Ptr panelSet;

	MovingFaceCenteredGrid2Ptr movingGrid;

	//保存每一帧从panel中点发射出来的粒子的索引
	ArrayI midleList;

	Eigen::VectorXd strength;

	Eigen::MatrixXd A;

private:

	//二维情况下，涡量是标量
	ArrayD _vorticities;


};

using FoamVortexDataPtr = shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::vorticities() {
	return _vorticities;
}

#endif
