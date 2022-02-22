#ifndef VORTEXPARTICLESSYSTEMDATA2_H
#define VORTEXPARTICLESSYSTEMDATA2_H

#include "ParticleSystemData2.h"

#include "../Geometry/ExplicitSurface2.h"

class VortexParticleSystemData2 : public ParticleSystemData2 {
public:
	VortexParticleSystemData2();

	virtual ~VortexParticleSystemData2();

	Array<double>& vorticities();

public:
	ExplicitSurface2Ptr panelSet;

	//����ÿһ֡��panel�е㷢����������ӵ�����
	Array<int> midleList;

private:

	//��ά����£������Ǳ���
	Array<double> _vorticities;

};

using VortexParticleSystemData2Ptr = shared_ptr<VortexParticleSystemData2>;


#endif