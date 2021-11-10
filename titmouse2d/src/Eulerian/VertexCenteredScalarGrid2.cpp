#include "VertexCenteredScalarGrid2.h"

VertexCenteredScalarGrid2::VertexCenteredScalarGrid2() {
}


VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    double initialValue) {
    
    //等下完善
    //resize(resolution, gridSpacing, origin, initialValue);
    //_data.resize(resolution + Vector2(1, 1), initialValue);
    //std::cout << _data.size().x() << endl;
}


//暂时不实现
VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
    const VertexCenteredScalarGrid2& other) {
    
}



Vector2<size_t> VertexCenteredScalarGrid2::dataSize() const {
    auto tempV = resolution();
    if (tempV.x != 0.0 && tempV.y != 0.0) {
        return resolution() + Vector2<size_t>(1, 1);
    }
    else {
        return Vector2<size_t>(0, 0);
    }
}

Vector2<double> VertexCenteredScalarGrid2::dataOrigin() const {
    return origin();
}




VertexCenteredScalarGrid2::Builder& VertexCenteredScalarGrid2::Builder::withResolution(const Vector2<size_t>& resolution) {
    _resolution = resolution;
    return *this;
}

VertexCenteredScalarGrid2::Builder& VertexCenteredScalarGrid2::Builder::withResolution(
    size_t resolutionX, size_t resolutionY) {
    _resolution.x = resolutionX;
    _resolution.y = resolutionY;
    return *this;
}


VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withGridSpacing(
    const Vector2<double>& gridSpacing) {
    _gridSpacing = gridSpacing;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withGridSpacing(
    double gridSpacingX, double gridSpacingY) {
    _gridSpacing.x = gridSpacingX;
    _gridSpacing.y = gridSpacingY;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withOrigin(const Vector2<double>& gridOrigin) {
    _gridOrigin = gridOrigin;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withOrigin(
    double gridOriginX, double gridOriginY) {
    _gridOrigin.x = gridOriginX;
    _gridOrigin.y = gridOriginY;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withInitialValue(double initialVal) {
    _initialVal = initialVal;
    return *this;
}

VertexCenteredScalarGrid2Ptr
VertexCenteredScalarGrid2::Builder::makeShared() const {
    return std::shared_ptr<VertexCenteredScalarGrid2>(
        new VertexCenteredScalarGrid2(
            _resolution,
            _gridSpacing,
            _gridOrigin,
            _initialVal),
        [](VertexCenteredScalarGrid2* obj) {
            delete obj;
        });
}


VertexCenteredScalarGrid2 VertexCenteredScalarGrid2::Builder::build() const {

    return VertexCenteredScalarGrid2(
        _resolution,
        _gridSpacing,
        _gridOrigin,
        _initialVal);

}

ScalarGrid2Ptr VertexCenteredScalarGrid2::Builder::build(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& gridOrigin,
    double initialVal) const {
    return std::shared_ptr<VertexCenteredScalarGrid2>(
        new VertexCenteredScalarGrid2(
            resolution,
            gridSpacing,
            gridOrigin,
            initialVal),
        [](VertexCenteredScalarGrid2* obj) {
            delete obj;
        });
}


VertexCenteredScalarGrid2::Builder VertexCenteredScalarGrid2::builder() {
    return Builder();
}
