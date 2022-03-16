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

	//����ÿһ֡��panel�е㷢����������ӵ�����
	ArrayI midleList;

	Eigen::VectorXd strength;

	Eigen::MatrixXd A;

private:

	//��ά����£������Ǳ���
	ArrayD _vorticities;


};

using FoamVortexDataPtr = shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::vorticities() {
	return _vorticities;
}

#endif
