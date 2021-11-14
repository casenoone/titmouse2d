#include "FaceCenteredGrid2.h"


FaceCenteredGrid2::FaceCenteredGrid2():
    _uLinearSampler(_dataU, Vector2<double>(1.0, 1.0), Vector2<double>()),
    _vLinearSampler(_dataV, Vector2<double>(1.0, 1.0), Vector2<double>())
{
    _dataOriginU = Vector2<double>(0.0, 0.5);
    _dataOriginV = Vector2<double>(0.5, 0.0);

}



FaceCenteredGrid2::FaceCenteredGrid2(const Vector2<size_t>& resolution,
    const Vector2<double>& origin,
    const Vector2<double>& initialValue) :
    _uLinearSampler(_dataU, Vector2<double>(2.0/resolution.x,2.0/resolution.y), uOrigin()),
    _vLinearSampler(_dataV, Vector2<double>(2.0 / resolution.x, 2.0 / resolution.y), vOrigin())
{

    auto gridSpacing = Vector2<double>(2.0 / resolution.x, 2.0 / resolution.y);
    resize(resolution, gridSpacing, origin, initialValue);
    _uLinearSampler._accessor = _dataU;
    _vLinearSampler._accessor = _dataV;

}

FaceCenteredGrid2::FaceCenteredGrid2(const FaceCenteredGrid2& other) {

   // set(other);
}



//这个函数暂时不实现
//void FaceCenteredGrid2::set(const FaceCenteredGrid2& other) {
//    setGrid(other);
//
//    _dataU.set(other._dataU);
//    _dataV.set(other._dataV);
//    _dataOriginU = other._dataOriginU;
//    _dataOriginV = other._dataOriginV;
//
//    resetSampler();
//}

FaceCenteredGrid2& FaceCenteredGrid2::operator=(
    const FaceCenteredGrid2& other) {
    //set(other);
    return *this;
}

double& FaceCenteredGrid2::u(size_t i, size_t j) { return _dataU(i, j); }

double& FaceCenteredGrid2::v(size_t i, size_t j) { return _dataV(i, j); }

Vector2<double> FaceCenteredGrid2::valueAtCellCenter(size_t i, size_t j) {

    return Vector2<double>(_dataU(i, j) + _dataU(i + 1, j),
        _dataV(i, j) + _dataV(i, j + 1)) * 0.5;
}



double FaceCenteredGrid2::divergenceAtCellCenter(size_t i, size_t j) {

    auto gs = gridSpacing();

    double leftU = _dataU(i, j);
    double rightU = _dataU(i + 1, j);
    double bottomV = _dataV(i, j);
    double topV = _dataV(i, j + 1);

    return (rightU - leftU) / gs.x + (topV - bottomV) / gs.y;
}


double FaceCenteredGrid2::curlAtCellCenter(size_t i, size_t j) {
    auto res = resolution();


    auto gs = gridSpacing();

    auto left = valueAtCellCenter((i > 0) ? i - 1 : i, j);
    auto right = valueAtCellCenter((i + 1 < res.x) ? i + 1 : i, j);
    auto bottom = valueAtCellCenter(i, (j > 0) ? j - 1 : j);
    auto top = valueAtCellCenter(i, (j + 1 < res.y) ? j + 1 : j);

    double Fx_ym = bottom.x;
    double Fx_yp = top.x;

    double Fy_xm = left.y;
    double Fy_xp = right.y;

    return 0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y;
}


VectorGrid2::DataPositionFunc FaceCenteredGrid2::uPosition() const {
    auto h = gridSpacing();

    return [this, h](double i, double j) -> Vector2<double> {
        return _dataOriginU + h * Vector2<double>({ i, j });
    };
}

VectorGrid2::DataPositionFunc FaceCenteredGrid2::vPosition() const {
    auto h = gridSpacing();

    return [this, h](double i, double j) -> Vector2<double> {
        return _dataOriginV + h * Vector2<double>({ i, j });
    };
}

Size2 FaceCenteredGrid2::uSize() {
    auto result = _dataU.dataSize();
    return result;
}

Size2 FaceCenteredGrid2::vSize() { return _dataV.dataSize(); }

Array2Ptr<double>& FaceCenteredGrid2::uDatas() {
    return _dataU;
}

Array2Ptr<double>& FaceCenteredGrid2::vDatas() {
    return _dataV;
}

Vector2<double> FaceCenteredGrid2::uOrigin() const { return _dataOriginU; }

Vector2<double> FaceCenteredGrid2::vOrigin() const { return _dataOriginV; }

Vector2<double> FaceCenteredGrid2::sample(const Vector2<double>& x) const {


    //这里有个问题，为什么不能直接调用_uLinearSampler？一调就baocuo
    auto uSample = _uLinearSampler;
    auto vSample = _vLinearSampler;

    auto u = uSample(_dataU, x);
    auto v = vSample(_dataV, x);
    Vector2<double> result(u, v);

    return result;
}

std::function<Vector2<double>(const Vector2<double>&)> FaceCenteredGrid2::sampler() const {
    return _sampler;
}


void FaceCenteredGrid2::fill(const Vector2<double>& value) {

    auto sizeU = uSize();

    for (size_t i = 0; i < sizeU.x; ++i) {
        for (size_t j = 0; j < sizeU.y; ++j) {
            _dataU(i, j) = value.x;
        }
    }


    auto sizeV = vSize();
    for (size_t i = 0; i < sizeV.x; ++i) {
        for (size_t j = 0; j < sizeV.y; ++j) {
            _dataV(i, j) = value.y;
        }
    }
}

//暂时不实现
double FaceCenteredGrid2::divergence(const Vector2<double>& x) const {
    //ssize_t i, j;
    //double fx, fy;
    //Vector2D cellCenterOrigin = origin() + 0.5 * gridSpacing();

    //Vector2D normalizedX = (x - cellCenterOrigin) / gridSpacing();

    //getBarycentric(normalizedX.x, 0, static_cast<ssize_t>(resolution().x) - 1,
    //    &i, &fx);
    //getBarycentric(normalizedX.y, 0, static_cast<ssize_t>(resolution().y) - 1,
    //    &j, &fy);

    //std::array<Point2UI, 4> indices;
    //std::array<double, 4> weights;

    //indices[0] = Point2UI(i, j);
    //indices[1] = Point2UI(i + 1, j);
    //indices[2] = Point2UI(i, j + 1);
    //indices[3] = Point2UI(i + 1, j + 1);

    //weights[0] = (1.0 - fx) * (1.0 - fy);
    //weights[1] = fx * (1.0 - fy);
    //weights[2] = (1.0 - fx) * fy;
    //weights[3] = fx * fy;

    //double result = 0.0;

    //for (int n = 0; n < 4; ++n) {
    //    result +=
    //        weights[n] * divergenceAtCellCenter(indices[n].x, indices[n].y);
    //}

    double result = 0;
    
    return result;
}

//暂时不实现
double FaceCenteredGrid2::curl(const Vector2<double>& x) const {
    //ssize_t i, j;
    //double fx, fy;
    //Vector2D cellCenterOrigin = origin() + 0.5 * gridSpacing();

    //Vector2D normalizedX = (x - cellCenterOrigin) / gridSpacing();

    //getBarycentric(normalizedX.x, 0, static_cast<ssize_t>(resolution().x) - 1,
    //    &i, &fx);
    //getBarycentric(normalizedX.y, 0, static_cast<ssize_t>(resolution().y) - 1,
    //    &j, &fy);

    //std::array<Point2UI, 4> indices;
    //std::array<double, 4> weights;

    //indices[0] = Point2UI(i, j);
    //indices[1] = Point2UI(i + 1, j);
    //indices[2] = Point2UI(i, j + 1);
    //indices[3] = Point2UI(i + 1, j + 1);

    //weights[0] = (1.0 - fx) * (1.0 - fy);
    //weights[1] = fx * (1.0 - fy);
    //weights[2] = (1.0 - fx) * fy;
    //weights[3] = fx * fy;

    //double result = 0.0;

    //for (int n = 0; n < 4; ++n) {
    //    result += weights[n] * curlAtCellCenter(indices[n].x, indices[n].y);
    //}
    double result = 0;
    return result;
}



void FaceCenteredGrid2::onResize(const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    const Vector2<double>& initialValue) {
    if (resolution != Vector2<size_t>(0, 0)) {
        auto ures = resolution + Vector2<size_t>(1, 0);
        auto vres = resolution + Vector2<size_t>(0, 1);
        _dataU.reSize(ures.x, ures.y, initialValue.x);
        _dataV.reSize(ures.x, ures.y, initialValue.x);
        
    }
    else {
        _dataU.reSize(0, 0, 0.0);
        _dataV.reSize(0, 0, 0.0);
    }
    _dataOriginU = origin + Vector2<double>(0.0, gridSpacing.y) * 0.5;
    _dataOriginV = origin + Vector2<double>(gridSpacing.x, 0.0) * 0.5;

    resetSampler();
}


//暂时不实现
void FaceCenteredGrid2::resetSampler() {
    //LinearArraySampler2<double, double> uSampler(_dataU.constAccessor(),
    //    gridSpacing(), _dataOriginU);
    //LinearArraySampler2<double, double> vSampler(_dataV.constAccessor(),
    //    gridSpacing(), _dataOriginV);

    //_uLinearSampler = uSampler;
    //_vLinearSampler = vSampler;

    //_sampler = [uSampler, vSampler](const Vector2D& x) -> Vector2D {
    //    double u = uSampler(x);
    //    double v = vSampler(x);
    //    return Vector2D(u, v);
    //};

}


FaceCenteredGrid2::Builder FaceCenteredGrid2::builder() { return Builder(); }




FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withResolution(
    const Vector2<size_t>& resolution) {
    _resolution = resolution;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withResolution(
    size_t resolutionX, size_t resolutionY) {
    _resolution.x = resolutionX;
    _resolution.y = resolutionY;
    return *this;
}



FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withOrigin(
    const Vector2<double>& gridOrigin) {
    _gridOrigin = gridOrigin;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withOrigin(
    double gridOriginX, double gridOriginY) {
    _gridOrigin.x = gridOriginX;
    _gridOrigin.y = gridOriginY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withInitialValue(
    const Vector2<double>& initialVal) {
    _initialVal = initialVal;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withInitialValue(
    double initialValX, double initialValY) {
    _initialVal.x = initialValX;
    _initialVal.y = initialValY;
    return *this;
}

FaceCenteredGrid2 FaceCenteredGrid2::Builder::build() const {
    return FaceCenteredGrid2(_resolution, _gridOrigin,
        _initialVal);
}

FaceCenteredGrid2Ptr FaceCenteredGrid2::Builder::makeShared() const {
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2(_resolution, _gridOrigin,
            _initialVal),
        [](FaceCenteredGrid2* obj) { delete obj; });
}

VectorGrid2Ptr FaceCenteredGrid2::Builder::build(
    const Vector2<size_t>& resolution, const Vector2<double>& gridOrigin, const Vector2<double>& initialVal) const {
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2(resolution, gridOrigin, initialVal),
        [](FaceCenteredGrid2* obj) { delete obj; });
}
