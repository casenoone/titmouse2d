#ifndef GRIDFLUIDSOLVER2_H
#define GRIDFLUIDSOLVER2_H

#include "../Vector2.hpp"
#include "GridSystemData2.h"

#include "../Collider2.h"

#include "SimplePressureSolver2.h"
#include "AdvectionSolver2.h"

class GridFluidSolver2 {
public:
	class Builder;

	GridFluidSolver2();

	~GridFluidSolver2();

	GridFluidSolver2(const Vector2I& resolution, const Vector2D& gridSpacing, const Vector2D& gridOrigin);

	//暂时写一个这样的接口，将来会专门抽象出一个流体发射器
	void setFluidSdf(const VertexCenteredScalarGrid2& _sdf);

	const VertexCenteredScalarGrid2Ptr sdf()const;

	VertexCenteredScalarGrid2Ptr& sdf();

	Vector2I resolution() const;

	Vector2D gridSpacing() const;

	Vector2D gridOrigin() const;

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

	static Builder builder();

protected:

	virtual void onBeginAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void onEndAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void computeExternalForces(double timeIntervalInSeconds);

	virtual void accumulateForces(double timeIntervalInSeconds);

	virtual void computePressure(double timeIntervalInSeconds);

	virtual void computeAdvection(double timeIntervalInSeconds);

	void computeGravity(double timeIntervalInSeconds);

	void beginAdvanceTimeStep(double timeIntervalInSeconds);

	void endAdvanceTimeStep(double timeIntervalInSeconds);

	void updateCollider(double timeIntervalInSeconds);

	const Vector2D& gravity() const;

	double cfl(double timeIntervalInSeconds) const;

	void setFluidCellNum();

	double maxCfl() const;

	//暂时不实现
   // void setMaxCfl(double newCfl);

	const GridSystemData2Ptr& gridSystemData() const;

	FaceCenteredGrid2Ptr velocity();


protected:
	//标记网格是否被粒子占用
	Array2I cellCenterMarkers;

	SimplePressureSolver2Ptr _pressureSolver;
	AdvectionSolver2Ptr _advectionSolver;

protected:
	Vector2D _gravity = Vector2D(0.0, -9.8);
	double _viscosityCoefficient = 0.0;
	double _maxCfl = 5.0;

	GridSystemData2Ptr _grids;
};


typedef std::shared_ptr<GridFluidSolver2> GridFluidSolver2Ptr;



template <typename DerivedBuilder>
class GridFluidSolverBuilderBase2 {
public:
	DerivedBuilder& withResolution(const Vector2I& resolution);

	DerivedBuilder& withOrigin(const Vector2D& gridOrigin);

protected:
	Vector2I _resolution;
	Vector2D _gridSpacing;
	Vector2D _gridOrigin;

};



template <typename T>
T& GridFluidSolverBuilderBase2<T>::withResolution(const Vector2I& resolution) {
	_resolution = resolution;
	return static_cast<T&>(*this);
}


template <typename T>
T& GridFluidSolverBuilderBase2<T>::withOrigin(const Vector2D& gridOrigin) {
	_gridOrigin = gridOrigin;
	return static_cast<T&>(*this);
}


class GridFluidSolver2::Builder final
	: public GridFluidSolverBuilderBase2<GridFluidSolver2::Builder> {
public:
	GridFluidSolver2 build() const;

	GridFluidSolver2Ptr makeShared() const {

		auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
		return std::make_shared<GridFluidSolver2>(_resolution, gridSpacing, _gridOrigin);
	}
};

#endif