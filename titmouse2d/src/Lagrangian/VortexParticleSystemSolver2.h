#ifndef VORTEXPARTICLESSYSTEMSOLVER2_H
#define VORTEXPARTICLESSYSTEMSOLVER2_H

#include "VortexParticleSystemData2.h"
#include "ParticleSystemSolver2.h"

class VortexParticleSystemSolver2 : public ParticleSystemSolver2 {

public:
	VortexParticleSystemSolver2();

	virtual ~VortexParticleSystemSolver2();

	virtual void timeIntegration(double timeIntervalInSeconds)override;


	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	VortexParticleSystemData2Ptr& vortexParticleData();

	Vector2<double> computeUSingle(const Vector2<double>& pos, int i)const;

	void setData(int numberOfParticles,
		ArrayPtr<Vector2<double>>& pos,
		int resolutionX,
		int resolutionY);

	void setPanels(ExplicitSurface2Ptr surfaces);

	void emitParticlesFromPanel();

private:
	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();


private:
	VortexParticleSystemData2Ptr _vortexParticleData;

};



#endif