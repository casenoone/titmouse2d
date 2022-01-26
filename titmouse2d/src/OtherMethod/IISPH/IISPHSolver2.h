#ifndef IISPHSOLVER2_H
#define IISPHSOLVER2_H

#include "../../Lagrangian/SphSystemSolver2.h"
#include "IISPHData2.h"

//�˺����뾶
const double iisphKR = 0.05;

//�����뾶
const double iisph_h = 0;

class IISphSolver2 : public SphSystemSolver2 {
public:
	IISphSolver2();

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	void setData(int numberOfParticles,
		ArrayPtr<Vector2<double>>& pos,
		int resolutionX,
		int resolutionY);

	IISphData2Ptr iisphData();

	//��һ���Ѿ�������ھ�����
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
	_particleSystemData = make_shared<IISphData2>();
	_iisphData = make_shared<IISphData2>();
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
	pressure.reSize(n);
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