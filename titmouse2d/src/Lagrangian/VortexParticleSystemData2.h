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

	//����ÿһ֡��panel�е㷢����������ӵ�����
	ArrayI midleList;

	Eigen::VectorXd strength;

	Eigen::MatrixXd A;

private:

	//��ά����£������Ǳ���
	ArrayD _vorticities;


};

using VortexParticleSystemData2Ptr = shared_ptr<VortexParticleSystemData2>;


#endif