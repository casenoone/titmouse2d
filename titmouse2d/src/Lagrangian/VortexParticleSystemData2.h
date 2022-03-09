#ifndef VORTEXPARTICLESSYSTEMDATA2_H
#define VORTEXPARTICLESSYSTEMDATA2_H
#include <Eigen/Dense>
#include "ParticleSystemData2.h"

#include "../Geometry/ExplicitSurface2.h"

class VortexParticleSystemData2 : public ParticleSystemData2 {
public:
	VortexParticleSystemData2();

	virtual ~VortexParticleSystemData2();

	ArrayD& vorticities();

public:
	ExplicitSurface2Ptr panelSet;

	//保存每一帧从panel中点发射出来的粒子的索引
	ArrayI midleList;

	Eigen::VectorXd strength;

	Eigen::MatrixXd A;

private:

	//二维情况下，涡量是标量
	ArrayD _vorticities;


};

using VortexParticleSystemData2Ptr = shared_ptr<VortexParticleSystemData2>;


#endif