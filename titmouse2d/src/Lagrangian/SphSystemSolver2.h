#ifndef SPHSYSTEMSOLVER2_H
#define SPHSYSTEMSOLVER2_H

#include "ParticleSystemSolver2.h"
#include "SphSystemData2.h"


class SphSystemSolver2 : public ParticleSystemSolver2 {
public:
	SphSystemSolver2();

	~SphSystemSolver2();

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	virtual void accumulateForces(double timeIntervalInSeconds)override;

	//setDataÓ¦Ð´³ÉÐéº¯Êý
	void setData(int numberOfParticles,
		Array<Vector2D>& pos,
		int resolutionX,
		int resolutionY);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();

	void accumulateNonPressureForces(double timeStepInSeconds);
	void accumulatePressureForce(double timeStepInSeconds);

	void accumulatePressureForce(Array<Vector2D> positions,
		ArrayD densities,
		ArrayD pressures,
		Array<Vector2D> pressureForces);

	void accumulateViscosityForce();

	void computePseudoViscosity();

	void computePressure();
public:
	SphSystemData2Ptr& sphSystemData();

private:
	SphSystemData2Ptr _sphSystemData;

};

double computePressureFromEos(
	double density,
	double targetDensity,
	double eosScale,
	double eosExponent,
	double negativePressureScale);

#endif