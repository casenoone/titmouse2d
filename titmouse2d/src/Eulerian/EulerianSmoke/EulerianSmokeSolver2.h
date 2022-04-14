#ifndef EULERIANSMOKE2_H
#define EULERIANSMOKE2_H

#include "../GridFluidSolver2.h"
#include "EulerianSmokeGridData2.h"

//ʵ������Visual Simulation of Smoke ,Ronald Fedkiw,Jos Stam, 2004
//��������ʹ�����޲�ֶ������������

const double smoke_alpha = 0.1;
const double smoke_beta = 8;
const double amb_tempra = 0.0;

class EulerianSmokeSolver2 : public GridFluidSolver2 {
public:

	class Builder;

	EulerianSmokeSolver2();

	EulerianSmokeSolver2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	virtual ~EulerianSmokeSolver2();

	EulerianSmokeGridData2Ptr eulerianSmokeData();

	void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	static Builder builder();

protected:
	void computePressure(double timeIntervalInSeconds)override;

	void computeAdvection(double timeIntervalInSeconds) override;

	void setMarkers();

	void setFluidCellNum();

	void addDensity();

	void addTemperature();

	//�����洢�����ı���������
	void accumulateVorticityConfinement();

	void accumulateBuoyancyForce(double timeIntervalInSeconds);

	virtual void computeExternalForces(double timeIntervalInSeconds)override;

private:
	EulerianSmokeGridData2Ptr _eulerianSmokeData;

private:
};

typedef std::shared_ptr<EulerianSmokeSolver2> EulerianSmokeSolver2Ptr;



inline EulerianSmokeSolver2::EulerianSmokeSolver2() {

}

inline EulerianSmokeSolver2::EulerianSmokeSolver2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) :GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	_grids = std::make_shared<EulerianSmokeGridData2>(resolution, gridSpacing, gridOrigin);
	_eulerianSmokeData = std::make_shared<EulerianSmokeGridData2>(resolution, gridSpacing, gridOrigin);
	_eulerianSmokeData = std::dynamic_pointer_cast<EulerianSmokeGridData2>(_grids);
}


inline EulerianSmokeSolver2::~EulerianSmokeSolver2() {

}

inline EulerianSmokeGridData2Ptr EulerianSmokeSolver2::eulerianSmokeData() {
	return _eulerianSmokeData;
}





inline void EulerianSmokeSolver2::computePressure(double timeIntervalInSeconds) {
	_pressureSolver->solve(velocity(), cellCenterMarkers);
}


inline void EulerianSmokeSolver2::computeAdvection(double timeIntervalInSeconds) {
	auto vel = _eulerianSmokeData->velocity();
	auto density = _eulerianSmokeData->densities();
	auto temperature = _eulerianSmokeData->temperature();

	_advectionSolver->solve(vel, vel, timeIntervalInSeconds);
	_advectionSolver->solve(vel, density, timeIntervalInSeconds);
	_advectionSolver->solve(vel, temperature, timeIntervalInSeconds);
}




inline void EulerianSmokeSolver2::accumulateVorticityConfinement() {

}


inline void EulerianSmokeSolver2::computeExternalForces(double timeIntervalInSeconds) {
	//smoke��ʵû��Ҫ������
	//computeGravity(timeIntervalInSeconds);

	accumulateBuoyancyForce(timeIntervalInSeconds);

	//accumulateVorticityConfinement();
}




class EulerianSmokeSolver2::Builder final
	: public GridFluidSolverBuilderBase2<EulerianSmokeSolver2::Builder> {
public:
	EulerianSmokeSolver2 build() const;

	EulerianSmokeSolver2Ptr makeShared() const;
};


#endif