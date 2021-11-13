#include "GridSystemData2.h"

GridSystemData2::GridSystemData2() {
}


GridSystemData2::~GridSystemData2() {
}


GridSystemData2::GridSystemData2(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin) {
    _velocity = std::make_shared<FaceCenteredGrid2>(resolution, gridSpacing, origin);
    resize(resolution, gridSpacing, origin);
}

GridSystemData2::GridSystemData2(const GridSystemData2& other) {
 
}


void GridSystemData2::resize(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin) {
    _resolution = resolution;
    _gridSpacing = gridSpacing;
    _origin = origin;

}



Vector2<size_t> GridSystemData2::resolution() const {
    return _resolution;
}

Vector2<double> GridSystemData2::gridSpacing() const {
    return _gridSpacing;
}

Vector2<double> GridSystemData2::origin() const {
    return _origin;
}


FaceCenteredGrid2Ptr& GridSystemData2::velocity() {
    return _velocity;
}