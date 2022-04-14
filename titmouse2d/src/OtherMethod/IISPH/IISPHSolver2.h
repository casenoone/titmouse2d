#ifndef IISPHSOLVER2_H
#define IISPHSOLVER2_H

#include "../../Lagrangian/SphSystemSolver2.h"
#include "IISPHData2.h"

//ºËº¯Êý°ë¾¶
const double iisphKR = 0.08;

//ËÑË÷°ë¾¶
const double iisph_h = 0;

class IISphSolver2 : public SphSystemSolver2 {
public:
	IISphSolver2();

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	void setData(int numberOfParticles,
		Array<Vector2D>& pos,
		int resolutionX,
		int resolutionY);

	IISphData2Ptr iisphData();

	void initDensity();

	void initPressure();

	void computeAdv(double timeIntervalInSeconds);

	void computeD_ii(double timeIntervalInSeconds);

	void computeA_ii(double timeIntervalInSeconds);

	void iterPressureSolver(double timeIntervalInSeconds);

	void timeIntegration(double timeIntervalInSeconds)override;

private:
	IISphData2Ptr _iisphData;
};


inline IISphSolver2::IISphSolver2() {
	_particleSystemData = std::make_shared<IISphData2>();
	_iisphData = std::make_shared<IISphData2>();
	_iisphData = std::dynamic_pointer_cast<IISphData2>(_particleSystemData);

}


inline IISphData2Ptr IISphSolver2::iisphData() {
	return _iisphData;
}


inline void IISphSolver2::initDensity() {
	_iisphData->initDensity();
}

inline void IISphSolver2::initPressure() {
	auto n = iisphData()->numberOfParticles();
	auto pressure = _iisphData->pressures();
	pressure.reSize(n, 0.0);
}

inline void IISphSolver2::computeAdv(double timeIntervalInSeconds) {

	ParticleSystemSolver2::accumlateExternalForces();
	auto forces = iisphData()->forces();
	auto velocities = iisphData()->velocities();
	auto n = iisphData()->numberOfParticles();

	for (int i = 0; i < n; ++i) {
		_newVelocities[i] = velocities[i] + timeIntervalInSeconds * forces[i];
	}
}




#endif