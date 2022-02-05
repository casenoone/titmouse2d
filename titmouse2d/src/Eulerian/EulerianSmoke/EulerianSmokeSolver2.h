#ifndef EULERIANSMOKE2_H
#define EULERIANSMOKE2_H

#include "../GridFluidSolver2.h"
#include "EulerianSmokeGridData2.h"

//实现论文Visual Simulation of Smoke ,Ronald Fedkiw,Jos Stam, 2004
//不过这里使用有限差分而不是有限体积

class EulerianSmokeSolver2 : public GridFluidSolver2 {
public:

	class Builder;

	EulerianSmokeSolver2();

	EulerianSmokeSolver2(
		const Vector2<size_t>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	virtual ~EulerianSmokeSolver2();

	void setMarkers();

	void setFluidCellNum();

	EulerianSmokeGridData2Ptr eulerianSmokeData();

	void addDensity();

	void addTemperature();

	void accumulateVorticityConfinement();

	void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	static Builder builder();

protected:
	void computePressure(double timeIntervalInSeconds)override;

	void computeAdvection(double timeIntervalInSeconds) override;

private:
	EulerianSmokeGridData2Ptr _eulerianSmokeData;

private:
};

typedef std::shared_ptr<EulerianSmokeSolver2> EulerianSmokeSolver2Ptr;



EulerianSmokeSolver2::EulerianSmokeSolver2() {

}

EulerianSmokeSolver2::EulerianSmokeSolver2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin) :GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	//这样写会出问题么？
	_grids = make_shared<EulerianSmokeGridData2>(resolution, gridSpacing, gridOrigin);
	_eulerianSmokeData = make_shared<EulerianSmokeGridData2>(resolution, gridSpacing, gridOrigin);
	_eulerianSmokeData = std::dynamic_pointer_cast<EulerianSmokeGridData2>(_grids);
}


EulerianSmokeSolver2::~EulerianSmokeSolver2() {

}

EulerianSmokeGridData2Ptr EulerianSmokeSolver2::eulerianSmokeData() {
	return _eulerianSmokeData;
}


void EulerianSmokeSolver2::setMarkers() {
	//cellCenterMarkers(xss, yss) = FLUID;
}

void EulerianSmokeSolver2::setFluidCellNum() {
	//vel->solveSystemMarker.reSize(resolution().x, resolution().y, -1);
	//for (int j = 0; j < size.y; ++j) {
	//	for (int i = 0; i < size.x; ++i) {
	//		//如果格子被流体占据
	//		if (cellCenterMarkers(i, j) == FLUID) {
	//			vel->solveSystemMarker(i, j) = nums;
	//			nums += 1;
	//		}
	//	}
	//}
}


void EulerianSmokeSolver2::computePressure(double timeIntervalInSeconds) {
	_pressureSolver->solve(velocity(), cellCenterMarkers);
}


void EulerianSmokeSolver2::computeAdvection(double timeIntervalInSeconds) {
	auto vel = _eulerianSmokeData->velocity();
	auto density = _eulerianSmokeData->densities();
	auto temperature = _eulerianSmokeData->temperature();

	_advectionSolver->solve(vel, vel, timeIntervalInSeconds);
	_advectionSolver->solve(vel, density, timeIntervalInSeconds);
	_advectionSolver->solve(vel, temperature, timeIntervalInSeconds);
}


void EulerianSmokeSolver2::addDensity() {

}


void EulerianSmokeSolver2::addTemperature() {

}


void EulerianSmokeSolver2::accumulateVorticityConfinement() {

}



class EulerianSmokeSolver2::Builder final
	: public GridFluidSolverBuilderBase2<EulerianSmokeSolver2::Builder> {
public:
	EulerianSmokeSolver2 build() const;

	EulerianSmokeSolver2Ptr makeShared() const;
};


#endif