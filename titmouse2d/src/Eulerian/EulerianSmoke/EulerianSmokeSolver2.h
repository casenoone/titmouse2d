#ifndef EULERIANSMOKE2_H
#define EULERIANSMOKE2_H

#include "../GridFluidSolver2.h"
#include "EulerianSmokeGridData2.h"

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

	static Builder builder();

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

}

void EulerianSmokeSolver2::setFluidCellNum() {

}









class EulerianSmokeSolver2::Builder final
	: public GridFluidSolverBuilderBase2<EulerianSmokeSolver2::Builder> {
public:
	EulerianSmokeSolver2 build() const;

	EulerianSmokeSolver2Ptr makeShared() const;
};


#endif