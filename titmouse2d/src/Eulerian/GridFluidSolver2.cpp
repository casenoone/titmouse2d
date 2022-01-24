#include "GridFluidSolver2.h"
#include "../../src/ConstVar.h"

GridFluidSolver2::GridFluidSolver2() {}

GridFluidSolver2::GridFluidSolver2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin) {


	_grids = std::make_shared<GridSystemData2>(resolution, gridSpacing, gridOrigin);
	_pressureSolver = make_shared<SimplePressureSolver2>();
	_advectionSolver = make_shared<AdvectionSolver2>();

}

GridFluidSolver2::~GridFluidSolver2() {}


void GridFluidSolver2::accumulateForces(double timeIntervalInSeconds) {

}


void GridFluidSolver2::setFluidSdf(const VertexCenteredScalarGrid2& _sdf) {
	_grids->sdf() = make_shared<VertexCenteredScalarGrid2>(_sdf);
}

const VertexCenteredScalarGrid2Ptr GridFluidSolver2::sdf()const {
	return _grids->sdf();
}

VertexCenteredScalarGrid2Ptr& GridFluidSolver2::sdf() {
	return _grids->sdf();
}



const Vector2<double>& GridFluidSolver2::gravity() const { return _gravity; }


//计算CFL条件数
double GridFluidSolver2::cfl(double timeIntervalInSeconds) const {
	auto vel = _grids->velocity();
	double maxVel = 0.0;

	auto size = vel->resolution();
	size_t sizeX = size.x;
	size_t sizeY = size.y;

	for (size_t i = 0; i < sizeX; ++i) {
		for (size_t j = 0; j < sizeY; ++j) {
			Vector2<double> v =
				vel->valueAtCellCenter(i, j) + _gravity * timeIntervalInSeconds;
			maxVel = std::max(maxVel, v.x);
			maxVel = std::max(maxVel, v.y);
		}
	}



	auto  gridSpacing = _grids->gridSpacing();
	double minGridSize = std::min(gridSpacing.x, gridSpacing.y);

	return maxVel * timeIntervalInSeconds / minGridSize;
}


double GridFluidSolver2::maxCfl() const { return _maxCfl; }


const GridSystemData2Ptr& GridFluidSolver2::gridSystemData() const {
	return _grids;
}


Vector2<size_t> GridFluidSolver2::resolution() const { return _grids->resolution(); }

Vector2<double> GridFluidSolver2::gridSpacing() const { return _grids->gridSpacing(); }

Vector2<double> GridFluidSolver2::gridOrigin() const { return _grids->origin(); }

FaceCenteredGrid2Ptr GridFluidSolver2::velocity() {
	return _grids->velocity();
}


//暂时不实现
//const Collider2Ptr& GridFluidSolver2::collider() const { return _collider; }

void GridFluidSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	if (_grids->resolution().x == 0 || _grids->resolution().y == 0) {

		return;
	}

	beginAdvanceTimeStep(timeIntervalInSeconds);
	computeExternalForces(timeIntervalInSeconds);
	computeAdvection(timeIntervalInSeconds);
	computePressure(timeIntervalInSeconds);
	endAdvanceTimeStep(timeIntervalInSeconds);
}


void GridFluidSolver2::onBeginAdvanceTimeStep(double timeIntervalInSeconds) {

}

void GridFluidSolver2::onEndAdvanceTimeStep(double timeIntervalInSeconds) {

}


void GridFluidSolver2::computeExternalForces(double timeIntervalInSeconds) {
	computeGravity(timeIntervalInSeconds);
}

void GridFluidSolver2::computePressure(double timeIntervalInSeconds) {
	_pressureSolver->solve(velocity(), cellCenterMarkers);

}


void GridFluidSolver2::computeAdvection(double timeIntervalInSeconds) {
	_advectionSolver->solve(velocity(), velocity(), timeIntervalInSeconds);

}


GridFluidSolver2::Builder GridFluidSolver2::builder() { return Builder(); }


void GridFluidSolver2::computeGravity(double timeIntervalInSeconds) {
	if (_gravity.getLengthSquared() > kEpsilonD) {
		auto& vel = _grids->velocity();

		if (std::abs(_gravity.y) > kEpsilonD) {

			auto sizeV = vel->vSize();
			size_t sizeVx = sizeV.x;
			size_t sizeVy = sizeV.y;
			for (size_t i = 0; i < sizeVx; ++i) {
				for (size_t j = 0; j < sizeVy; ++j) {
					vel->v(i, j) += timeIntervalInSeconds * _gravity.y;
				}
			}


		}
	}
}


void GridFluidSolver2::beginAdvanceTimeStep(double timeIntervalInSeconds) {
	onBeginAdvanceTimeStep(timeIntervalInSeconds);
}


void GridFluidSolver2::endAdvanceTimeStep(double timeIntervalInSeconds) {

	onEndAdvanceTimeStep(timeIntervalInSeconds);

}


//暂时不实现
void GridFluidSolver2::updateCollider(double timeIntervalInSeconds) {
	/* if (_collider != nullptr) {
		 _collider->update(0, timeIntervalInSeconds);
	 }*/
}


void GridFluidSolver2::setFluidCellNum() {
	int nums = 0;
	auto size = resolution();
	auto vel = velocity();

	vel->solveSystemMarker.reSize(resolution().x, resolution().y, -1);

	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			//如果格子被流体占据
			if (cellCenterMarkers(i, j) == FLUID) {
				vel->solveSystemMarker(i, j) = nums;
				nums += 1;
			}
		}
	}
}

