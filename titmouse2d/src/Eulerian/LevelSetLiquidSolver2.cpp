#include "LevelSetLiquidSolver2.h"

#include "../ArrayUtils.hpp"
#include "sdf_utils.h"

#include "../ConstVar.h"

LevelSetLiquidSolver2::LevelSetLiquidSolver2() {

}


LevelSetLiquidSolver2::~LevelSetLiquidSolver2() {

}


LevelSetLiquidSolver2::LevelSetLiquidSolver2(const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& gridOrigin) : GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

    _levelsetSolver = make_shared <SimpleLevelSetSolver2>();

}



void LevelSetLiquidSolver2::extrapolateVelocityToAir() {

    auto this_sdf = sdf();
    auto vel = gridSystemData()->velocity();

    auto u = vel->uDatas();
    auto v = vel->vDatas();
    auto uPos = vel->uPosition();
    auto vPos = vel->vPosition();

    Array2Ptr<int> uMarker, vMarker;
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
                u(i, j) = 0.0;
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
                v(i, j) = 0.0;
            }
        }
    }


    int depth = std::ceil(maxCfl());
    extrapolateToRegion(u, uMarker, depth, u);
    extrapolateToRegion(v, uMarker, depth, v);

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
                cellCenterMarkers(i,j) = FLUID;
            }
            else {
                cellCenterMarkers(i, j) = AIR;
            }
        }
    }
    
}



LevelSetLiquidSolver2 LevelSetLiquidSolver2::Builder::build() const {
    auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
    return LevelSetLiquidSolver2(_resolution, gridSpacing, _gridOrigin);
}


LevelSetLiquidSolver2Ptr LevelSetLiquidSolver2::Builder::makeShared() const {
    cout << 7878787887878 << endl;
    auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
    return std::shared_ptr<LevelSetLiquidSolver2>(
        new LevelSetLiquidSolver2(
            _resolution,
            gridSpacing,
            _gridOrigin),
        [](LevelSetLiquidSolver2* obj) {
            delete obj;
        });
}

