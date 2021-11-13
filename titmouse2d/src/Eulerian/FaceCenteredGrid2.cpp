#include "FaceCenteredGrid2.h"


FaceCenteredGrid2::FaceCenteredGrid2():
    _uLinearSampler(_dataU, Vector2(1, 1), Vector2(0, 0)),
    _vLinearSampler(_dataV, Vector2(1, 1), Vector2(0, 0)) 
{
    _dataOriginU = Vector2<double>(0.0, 0.5);
    _dataOriginV = Vector2<double>(0.5, 0.0);

}



FaceCenteredGrid2::FaceCenteredGrid2(const Vector2& resolution,
    const Vector2& gridSpacing,
    const Vector2& origin,
    const Vector2& initialValue) :
    _uLinearSampler(_dataU, gridSpacing, uOrigin()),
    _vLinearSampler(_dataV, gridSpacing, vOrigin())
{

    //这里还对线性采样器初始化了
    //这里暂时不实现
    //与上面那个构造函数类似

    //傻逼函数，这个函数执行完之后才会对data进行初始化
    resize(resolution, gridSpacing, origin, initialValue);
    _uLinearSampler._scalarAccessor = _dataU;
    _vLinearSampler._scalarAccessor = _dataV;

    solveSystemMarker.resize(resolution, -1);

}

FaceCenteredGrid2::FaceCenteredGrid2(const FaceCenteredGrid2& other) {

    set(other);
}

void FaceCenteredGrid2::swap(Grid2* other) {

    //这是干啥呢？
    FaceCenteredGrid2* sameType = dynamic_cast<FaceCenteredGrid2*>(other);

    if (sameType != nullptr) {
        swapGrid(sameType);

        _dataU.swap(sameType->_dataU);
        _dataV.swap(sameType->_dataV);
        std::swap(_dataOriginU, sameType->_dataOriginU);
        std::swap(_dataOriginV, sameType->_dataOriginV);
        std::swap(_uLinearSampler, sameType->_uLinearSampler);
        std::swap(_vLinearSampler, sameType->_vLinearSampler);
        std::swap(_sampler, sameType->_sampler);
    }
}


void FaceCenteredGrid2::set(const FaceCenteredGrid2& other) {
    setGrid(other);

    _dataU.set(other._dataU);
    _dataV.set(other._dataV);
    _dataOriginU = other._dataOriginU;
    _dataOriginV = other._dataOriginV;

    resetSampler();
}

FaceCenteredGrid2& FaceCenteredGrid2::operator=(
    const FaceCenteredGrid2& other) {
    set(other);
    return *this;
}

double& FaceCenteredGrid2::u(size_t i, size_t j) { return _dataU(i, j); }

double& FaceCenteredGrid2::v(size_t i, size_t j) { return _dataV(i, j); }

Vector2 FaceCenteredGrid2::valueAtCellCenter(size_t i, size_t j) {

    return Vector2(_dataU(i, j) + _dataU(i + 1, j),
        _dataV(i, j) + _dataV(i, j + 1)) * 0.5;
}



double FaceCenteredGrid2::divergenceAtCellCenter(size_t i, size_t j) {

    Vector2 gs = gridSpacing();

    double leftU = _dataU(i, j);
    double rightU = _dataU(i + 1, j);
    double bottomV = _dataV(i, j);
    double topV = _dataV(i, j + 1);

    return (rightU - leftU) / gs.x() + (topV - bottomV) / gs.y();
}


double FaceCenteredGrid2::curlAtCellCenter(size_t i, size_t j) {
    Vector2 res = resolution();


    Vector2 gs = gridSpacing();

    Vector2 left = valueAtCellCenter((i > 0) ? i - 1 : i, j);
    Vector2 right = valueAtCellCenter((i + 1 < res.x()) ? i + 1 : i, j);
    Vector2 bottom = valueAtCellCenter(i, (j > 0) ? j - 1 : j);
    Vector2 top = valueAtCellCenter(i, (j + 1 < res.y()) ? j + 1 : j);

    double Fx_ym = bottom.x();
    double Fx_yp = top.x();

    double Fy_xm = left.y();
    double Fy_xp = right.y();

    return 0.5 * (Fy_xp - Fy_xm) / gs.x() - 0.5 * (Fx_yp - Fx_ym) / gs.y();
}


VectorGrid2::DataPositionFunc FaceCenteredGrid2::uPosition() const {
    Vector2 h = gridSpacing();

    return [this, h](double i, double j) -> Vector2 {
        return _dataOriginU + h * Vector2({ i, j });
    };
}

VectorGrid2::DataPositionFunc FaceCenteredGrid2::vPosition() const {
    Vector2 h = gridSpacing();

    return [this, h](double i, double j) -> Vector2 {
        return _dataOriginV + h * Vector2({ i, j });
    };
}

Vector2 FaceCenteredGrid2::uSize() {
    auto result = _dataU.size();
    return result;
}

Vector2 FaceCenteredGrid2::vSize() { return _dataV.size(); }

DoubleArray2& FaceCenteredGrid2::uDatas() {
    return _dataU;
}

DoubleArray2& FaceCenteredGrid2::vDatas() {
    return _dataV;
}

Vector2 FaceCenteredGrid2::uOrigin() const { return _dataOriginU; }

Vector2 FaceCenteredGrid2::vOrigin() const { return _dataOriginV; }

Vector2 FaceCenteredGrid2::sample(const Vector2& x) const {


    //这里有个问题，为什么不能直接调用_uLinearSampler？
    auto uSample = _uLinearSampler;
    auto vSample = _vLinearSampler;

    auto u = uSample(_dataU, x);
    auto v = vSample(_dataV, x);
    Vector2 result(u, v);

    return result;
}

std::function<Vector2(const Vector2&)> FaceCenteredGrid2::sampler() const {
    return _sampler;
}

std::shared_ptr<VectorGrid2> FaceCenteredGrid2::clone() const {
    return CLONE_W_CUSTOM_DELETER(FaceCenteredGrid2);
}

void FaceCenteredGrid2::fill(const Vector2& value) {

    auto sizeU = uSize();

    for (size_t i = 0; i < sizeU.x(); ++i) {
        for (size_t j = 0; j < sizeU.y(); ++j) {
            _dataU(i, j) = value.x();
        }
    }


    auto sizeV = vSize();
    for (size_t i = 0; i < sizeV.x(); ++i) {
        for (size_t j = 0; j < sizeV.y(); ++j) {
            _dataV(i, j) = value.y();
        }
    }
}

double FaceCenteredGrid2::divergence(const Vector2& x) const {
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
    cout << "FaceCenteredGrid2::divergence 尚未实现" << endl;
    return result;
}


double FaceCenteredGrid2::curl(const Vector2& x) const {
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
    cout << "FaceCenteredGrid2::curl 尚未实现" << endl;
    return result;
}



void FaceCenteredGrid2::onResize(const Vector2& resolution,
    const Vector2& gridSpacing,
    const Vector2& origin,
    const Vector2& initialValue) {
    if (resolution != Vector2(0, 0)) {
        _dataU.resize(resolution + Vector2(1, 0), initialValue.x());
        _dataV.resize(resolution + Vector2(0, 1), initialValue.y());
    }
    else {
        _dataU.resize(Vector2(0, 0), 0.0);
        _dataV.resize(Vector2(0, 0), 0.0);
    }
    _dataOriginU = origin + Vector2(0.0, gridSpacing.y()) * 0.5;
    _dataOriginV = origin + Vector2(gridSpacing.x(), 0.0) * 0.5;

    resetSampler();
}

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

    cout << "FaceCenteredGrid2::resetSampler 尚未实现" << endl;
}


FaceCenteredGrid2::Builder FaceCenteredGrid2::builder() { return Builder(); }


void FaceCenteredGrid2::getData(std::vector<double>* data) {
    /////size_t size = uSize().x * uSize().y + vSize().x * vSize().y;
    ////data->resize(size);
    ////size_t cnt = 0;
    ////_dataU.forEach([&](double value) { (*data)[cnt++] = value; });
    ////_dataV.forEach([&](double value) { (*data)[cnt++] = value; });

    cout << "FaceCenteredGrid2::getData 尚未实现" << endl;
}

void FaceCenteredGrid2::setData(const std::vector<double>& data) {

    //size_t cnt = 0;
    //_dataU.forEachIndex(
    //    [&](size_t i, size_t j) { _dataU(i, j) = data[cnt++]; });
    //_dataV.forEachIndex(
    //    [&](size_t i, size_t j) { _dataV(i, j) = data[cnt++]; });
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withResolution(
    const Vector2& resolution) {
    _resolution = resolution;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withResolution(
    size_t resolutionX, size_t resolutionY) {
    _resolution.x() = resolutionX;
    _resolution.y() = resolutionY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withGridSpacing(
    const Vector2& gridSpacing) {
    _gridSpacing = gridSpacing;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withGridSpacing(
    double gridSpacingX, double gridSpacingY) {
    _gridSpacing.x() = gridSpacingX;
    _gridSpacing.y() = gridSpacingY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withOrigin(
    const Vector2& gridOrigin) {
    _gridOrigin = gridOrigin;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withOrigin(
    double gridOriginX, double gridOriginY) {
    _gridOrigin.x() = gridOriginX;
    _gridOrigin.y() = gridOriginY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withInitialValue(
    const Vector2& initialVal) {
    _initialVal = initialVal;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::withInitialValue(
    double initialValX, double initialValY) {
    _initialVal.x() = initialValX;
    _initialVal.y() = initialValY;
    return *this;
}

FaceCenteredGrid2 FaceCenteredGrid2::Builder::build() const {
    return FaceCenteredGrid2(_resolution, _gridSpacing, _gridOrigin,
        _initialVal);
}

FaceCenteredGrid2Ptr FaceCenteredGrid2::Builder::makeShared() const {
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2(_resolution, _gridSpacing, _gridOrigin,
            _initialVal),
        [](FaceCenteredGrid2* obj) { delete obj; });
}

VectorGrid2Ptr FaceCenteredGrid2::Builder::build(
    const Vector2& resolution, const Vector2& gridSpacing,
    const Vector2& gridOrigin, const Vector2& initialVal) const {
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2(resolution, gridSpacing, gridOrigin, initialVal),
        [](FaceCenteredGrid2* obj) { delete obj; });
}
