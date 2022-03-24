#include "FaceCenteredGrid2.h"
#include "../ConstVar.h"

#include <array>

FaceCenteredGrid2::FaceCenteredGrid2() :
	_uLinearSampler(_dataU, Vector2D(1.0, 1.0), Vector2D()),
	_vLinearSampler(_dataV, Vector2D(1.0, 1.0), Vector2D())
{
	_dataOriginU = Vector2D(0.0, 0.5);
	_dataOriginV = Vector2D(0.5, 0.0);


}



FaceCenteredGrid2::FaceCenteredGrid2(const Vector2I& resolution,
	const Vector2D& origin,
	const Vector2D& initialValue)
{
	auto gridSpacing = Vector2D(2.0 / resolution.x, 2.0 / resolution.y);
	resize(resolution, gridSpacing, origin, initialValue);

	_uLinearSampler.resize(_dataU, gridSpacing, _dataOriginU);
	_vLinearSampler.resize(_dataV, gridSpacing, _dataOriginV);
	solveSystemMarker.reSize(resolution.x, resolution.y, AIR);
}

FaceCenteredGrid2::FaceCenteredGrid2(const FaceCenteredGrid2& other) {

	// set(other);
}



double& FaceCenteredGrid2::u(int i, int j) { return _dataU(i, j); }

double& FaceCenteredGrid2::v(int i, int j) { return _dataV(i, j); }

Vector2D FaceCenteredGrid2::valueAtCellCenter(int i, int j) const {

	return Vector2D(_dataU.lookAt(i, j) + _dataU.lookAt(i + 1, j),
		_dataV.lookAt(i, j) + _dataV.lookAt(i, j + 1)) * 0.5;
}



double FaceCenteredGrid2::divergenceAtCellCenter(int i, int j) {

	auto gs = gridSpacing();

	double leftU = _dataU(i, j);
	double rightU = _dataU(i + 1, j);
	double bottomV = _dataV(i, j);
	double topV = _dataV(i, j + 1);

	return (rightU - leftU) / gs.x + (topV - bottomV) / gs.y;
}


double FaceCenteredGrid2::curlAtCellCenter(int i, int j) const {
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


VectorGrid2<Vector2D>::DataPositionFunc FaceCenteredGrid2::uPosition() const {
	auto h = gridSpacing();

	return [this, h](double i, double j) -> Vector2D {
		return _dataOriginU + h * Vector2D({ i, j });
	};
}

VectorGrid2<Vector2D>::DataPositionFunc FaceCenteredGrid2::vPosition() const {
	auto h = gridSpacing();

	return [this, h](double i, double j) -> Vector2D {
		return _dataOriginV + h * Vector2D({ i, j });
	};
}

Size2 FaceCenteredGrid2::uSize() {
	auto result = _dataU.dataSize();
	return result;
}

Size2 FaceCenteredGrid2::vSize() { return _dataV.dataSize(); }

Array2D& FaceCenteredGrid2::uDatas() {
	return _dataU;
}

Array2D& FaceCenteredGrid2::vDatas() {
	return _dataV;
}

Vector2D FaceCenteredGrid2::uOrigin() const { return _dataOriginU; }

Vector2D FaceCenteredGrid2::vOrigin() const { return _dataOriginV; }

Vector2D FaceCenteredGrid2::sample(const Vector2D& x) const {

	auto u = _uLinearSampler(_dataU, x);
	auto v = _vLinearSampler(_dataV, x);
	Vector2D result(u, v);
	return result;
}

std::function<Vector2D(const Vector2D&)> FaceCenteredGrid2::sampler() const {
	return _sampler;
}


void FaceCenteredGrid2::fill(const Vector2D& value) {

	auto sizeU = uSize();

	for (int i = 0; i < sizeU.x; ++i) {
		for (int j = 0; j < sizeU.y; ++j) {
			_dataU(i, j) = value.x;
		}
	}


	auto sizeV = vSize();
	for (int i = 0; i < sizeV.x; ++i) {
		for (int j = 0; j < sizeV.y; ++j) {
			_dataV(i, j) = value.y;
		}
	}
}

//暂时不实现
double FaceCenteredGrid2::divergence(const Vector2D& x) const {
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


double FaceCenteredGrid2::curl(const Vector2D& x) const {
	int i, j;
	double fx, fy;
	Vector2D cellCenterOrigin = origin() + 0.5 * gridSpacing();

	Vector2D normalizedX = (x - cellCenterOrigin) / gridSpacing();

	getBarycentric(normalizedX.x, 0, static_cast<int>(resolution().x) - 1,
		&i, &fx);
	getBarycentric(normalizedX.y, 0, static_cast<int>(resolution().y) - 1,
		&j, &fy);

	std::array<Vector2I, 4> indices;
	std::array<double, 4> weights;

	indices[0] = Vector2I(i, j);
	indices[1] = Vector2I(i + 1, j);
	indices[2] = Vector2I(i, j + 1);
	indices[3] = Vector2I(i + 1, j + 1);

	weights[0] = (1.0 - fx) * (1.0 - fy);
	weights[1] = fx * (1.0 - fy);
	weights[2] = (1.0 - fx) * fy;
	weights[3] = fx * fy;

	double result = 0.0;

	for (int n = 0; n < 4; ++n) {
		result += weights[n] * curlAtCellCenter(indices[n].x, indices[n].y);
	}

	return result;
}



void FaceCenteredGrid2::onResize(const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin,
	const Vector2D& initialValue) {
	if (resolution != Vector2I(0, 0)) {
		auto ures = resolution + Vector2I(1, 0);
		auto vres = resolution + Vector2I(0, 1);
		_dataU.reSize(ures.x, ures.y, initialValue.x);
		_dataV.reSize(vres.x, vres.y, initialValue.y);

	}
	else {
		_dataU.reSize(0, 0, 0.0);
		_dataV.reSize(0, 0, 0.0);
	}
	_dataOriginU = origin + Vector2D(0.0, gridSpacing.y) * 0.5;
	_dataOriginV = origin + Vector2D(gridSpacing.x, 0.0) * 0.5;

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


