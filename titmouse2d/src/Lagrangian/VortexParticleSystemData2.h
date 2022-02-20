#ifndef VORTEXPARTICLESSYSTEMDATA2_H
#define VORTEXPARTICLESSYSTEMDATA2_H

#include "ParticleSystemData2.h"

class VortexParticleSystemData2 : public ParticleSystemData2 {
public:
	VortexParticleSystemData2();

	virtual ~VortexParticleSystemData2();

	ArrayPtr<double>& vorticities();


private:

	//��ά����£������Ǳ���
	ArrayPtr<double> _vorticities;

};

using VortexParticleSystemData2Ptr = shared_ptr<VortexParticleSystemData2>;


#endif