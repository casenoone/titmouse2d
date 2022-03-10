#include "MovingFaceCenteredGrid2.h"

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