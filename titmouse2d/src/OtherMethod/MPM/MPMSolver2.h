#ifndef MPMSOLVER2_H
#define MPMSOLVER2_H

#include "MPMData2.h"

//这里实现的是MLS-MPM 

class MPMSolver2 {
public:
	MPMSolver2() = default;

	const MPMData2Ptr const mpmData()const;

	void onAdvanceTimeStep(double timeIntervalInSeconds);

	void setData(int numberOfParticles,
		ArrayPtr<Vector2<double>>& pos,
		int resolutionX,
		int resolutionY);

	void transferFromParticlesToGrids(double timeIntervalInSeconds);

	void transferFromGridsToParticles(double timeIntervalInSeconds);

private:
	MPMData2Ptr _mpmData;

	double MPM_E = 400.0;

	//记得初始化它
	double mpm_vol;

	double mpm_mass;

};



inline const MPMData2Ptr const MPMSolver2::mpmData()const {
	return _mpmData;
}

#endif
