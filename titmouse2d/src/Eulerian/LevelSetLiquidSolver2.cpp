#include "LevelSetLiquidSolver2.h"

#include "../ArrayUtils.hpp"
#include "sdf_utils.h"

#include "../ConstVar.h"

LevelSetLiquidSolver2::LevelSetLiquidSolver2() {

}


LevelSetLiquidSolver2::~LevelSetLiquidSolver2() {

}


LevelSetLiquidSolver2::LevelSetLiquidSolver2(const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) : GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	_levelsetSolver = std::make_shared <SimpleLevelSetSolver2>();

}



void LevelSetLiquidSolver2::extrapolateVelocityToAir() {

	auto this_sdf = sdf();
	auto vel = gridSystemData()->velocity();

	auto u = vel->uDatas();
	auto v = vel->vDatas();
	auto uPos = vel->uPosition();
	auto vPos = vel->vPosition();

	Array2I uMarker, vMarker;
	uMarker.reSize(u.dataSize().x, u.dataSize().y);
	vMarker.reSize(v.dataSize().x, v.dataSize().y);

	//此处循环可并行
	for (int i = 0; i < uMarker.dataSize().x; ++i) {
		for (int j = 0; j < uMarker.dataSize().y; ++j) {
			if (isInsideSdf(this_sdf->sample(uPos(i, j)))) {
				uMarker(i, j) = 1;
			}
			else {
				uMarker(i, j) = 0;
			}
		}
	}



	for (int i = 0; i < vMarker.dataSize().x; ++i) {
		for (int j = 0; j < vMarker.dataSize().y; ++j) {
			if (isInsideSdf(this_sdf->sample(vPos(i, j)))) {
				vMarker(i, j) = 1;
			}
			else {
				vMarker(i, j) = 0;
			}
		}
	}


	int depth = static_cast<int>(std::ceil(maxCfl()));
	extrapolateToRegion(u, uMarker, depth, u);
	extrapolateToRegion(v, vMarker, depth, v);

	for (int i = 0; i < u.dataSize().x; ++i) {
		for (int j = 0; j < u.dataSize().y; ++j) {
			if (i <= 0 || i >= u.dataSize().x - 1) {
				//u(i, j) = 0;
			}

			/*if (j <= 0 || j >= flow->vSize().y - 1) {
				v(i, j) = 0;
			}*/
		}
	}


	for (int i = 0; i < v.dataSize().x; ++i) {
		for (int j = 0; j < v.dataSize().y; ++j) {

			if (j <= 0 || j >= v.dataSize().y - 1) {
				//v(i, j) = 0;
			}
		}
	}

}


//根据sdf为网格打上标记
void LevelSetLiquidSolver2::setMarkers() {
	auto this_sdf = sdf();
	auto resolution = this_sdf->resolution();


	cellCenterMarkers.reSize(resolution.x, resolution.y, AIR);

	for (int i = 0; i < resolution.x; ++i) {
		for (int j = 0; j < resolution.y; ++j) {
			auto posFunc = this_sdf->dataPosition();
			auto p = posFunc(i, j);
			if (isInsideSdf(this_sdf->sample(p))) {
				cellCenterMarkers(i, j) = FLUID;
			}
			else {
				cellCenterMarkers(i, j) = AIR;
			}
		}
	}

}



void LevelSetLiquidSolver2::computeSdfAdvection(double timeIntervalInSeconds) {
	_advectionSolver->solve(velocity(), sdf(), timeIntervalInSeconds);
}


void LevelSetLiquidSolver2::onBeginAdvanceTimeStep(double timeIntervalInSeconds) {
	//外推速度场
	extrapolateVelocityToAir();

	//计算流体网格标签以及流体网格编号
	setMarkers();
	setFluidCellNum();
}



void LevelSetLiquidSolver2::onEndAdvanceTimeStep(double timeIntervalInSeconds) {
	extrapolateVelocityToAir();
	computeSdfAdvection(timeIntervalInSeconds);

	//所以说这玩意儿到底起作用没？也看不出来啊
	_levelsetSolver->reinitialize(*sdf(), 5, sdf());
}

LevelSetLiquidSolver2::Builder LevelSetLiquidSolver2::builder() {
	return Builder();
}



LevelSetLiquidSolver2 LevelSetLiquidSolver2::Builder::build() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return LevelSetLiquidSolver2(_resolution, gridSpacing, _gridOrigin);
}


LevelSetLiquidSolver2Ptr LevelSetLiquidSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<LevelSetLiquidSolver2>(
		new LevelSetLiquidSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](LevelSetLiquidSolver2* obj) {
			delete obj;
		});
}

