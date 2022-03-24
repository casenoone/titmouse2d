#include "MovingFaceCenteredGrid2.h"

#include <array>

MovingFaceCenteredGrid2::MovingFaceCenteredGrid2(
	const Vector2I& resolution_,
	const BoundingBox2& domain_) :_resolution(resolution_), _domain(domain_) {

	_gridSpacing.x = (domain_.upperCorner.x - _domain.lowerCorner.x) / resolution_.x;
	_gridSpacing.y = (domain_.upperCorner.y - _domain.lowerCorner.y) / resolution_.y;

	auto ures = resolution_ + Vector2I(1, 0);
	auto vres = resolution_ + Vector2I(0, 1);
	_dataU.reSize(ures.x, ures.y);
	_dataV.reSize(vres.x, vres.y);

	_dataOriginU = domain_.lowerCorner + Vector2D(0.0, _gridSpacing.y) * 0.5;
	_dataOriginV = domain_.lowerCorner + Vector2D(_gridSpacing.x, 0.0) * 0.5;

	_uLinearSampler.resize(_dataU, _gridSpacing, _dataOriginU);
	_vLinearSampler.resize(_dataV, _gridSpacing, _dataOriginV);
}


double MovingFaceCenteredGrid2::curl(const Vector2D& x) const {
	int i, j;
	double fx, fy;
	Vector2D cellCenterOrigin = _domain.lowerCorner + 0.5 * gridSpacing();

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

double MovingFaceCenteredGrid2::curlAtCellCenter(int i, int j) const {
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



void MovingFaceCenteredGrid2::fill(const Vector2D& value) {
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