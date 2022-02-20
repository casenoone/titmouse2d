#ifndef VORTEXPARTICLESSYSTEMDATA2_H
#define VORTEXPARTICLESSYSTEMDATA2_H

#include "ParticleSystemData2.h"

class VortexParticleSystemData2 : public ParticleSystemData2 {
public:
	VortexParticleSystemData2();

	virtual ~VortexParticleSystemData2();

	ArrayPtr<double>& vorticities();


private:

	//二维情况下，涡量是标量
	ArrayPtr<double> _vorticities;

};

using VortexParticleSystemData2Ptr = shared_ptr<VortexParticleSystemData2>;


#endif