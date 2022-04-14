#ifndef VORTEXPARTICLESSYSTEMDATA2_H
#define VORTEXPARTICLESSYSTEMDATA2_H
#include "ParticleSystemData2.h"

#include "../Geometry/ExplicitSurface2.h"

class VortexParticleSystemData2 : public ParticleSystemData2 {
public:
	VortexParticleSystemData2();

	virtual ~VortexParticleSystemData2();

	ArrayD& vorticities();



private:

	//��ά����£������Ǳ���
	ArrayD _vorticities;


};

using VortexParticleSystemData2Ptr = std::shared_ptr<VortexParticleSystemData2>;


#endif