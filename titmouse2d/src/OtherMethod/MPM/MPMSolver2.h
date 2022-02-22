#ifndef MPMSOLVER2_H
#define MPMSOLVER2_H

#include "MPMData2.h"

//这里实现的是MLS-MPM 

class MPMSolver2 {
public:
	MPMSolver2() = default;

	MPMSolver2(
		const Vector2<int>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	const MPMData2Ptr  mpmData()const;

	void onAdvanceTimeStep(double timeIntervalInSeconds);

	void setData(int numberOfParticles,
		Array<Vector2<double>>& pos,
		int resolutionX,
		int resolutionY);

	void transferFromParticlesToGrids(double timeIntervalInSeconds);

	void transferFromGridsToParticles(double timeIntervalInSeconds);

private:
	void initMPMData();

	MPMData2Ptr _mpmData;

	double MPM_E = 400;

	double mpm_vol;

	double mpm_mass;

};



inline const MPMData2Ptr  MPMSolver2::mpmData()const {
	return _mpmData;
}


inline MPMSolver2::MPMSolver2(
	const Vector2<int>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin) {
	_mpmData = make_shared<MPMData2>(resolution, gridSpacing, gridOrigin);

}

#endif
