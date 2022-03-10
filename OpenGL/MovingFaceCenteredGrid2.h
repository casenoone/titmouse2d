#ifndef MOVINGFACECENTEREDGRID2_H
#define MOVINGFACECENTEREDGRID2_H

#include <iostream>
#include <functional>

#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Size2.h"
#include "../titmouse2d/src/boundingbox2.h"
#include "../titmouse2d/src/Array2.hpp"
#include "../titmouse2d/src/Eulerian/ArraySampler2.hpp"

class MovingFaceCenteredGrid2 {
public:

	using DataPositionFunc = std::function<Vector2D(int, int)>;

	MovingFaceCenteredGrid2() = default;

	MovingFaceCenteredGrid2(
		const Vector2I& resolutin,
		const BoundingBox2& domain_);

	double& u(int i, int j);
	double& v(int i, int j);

	DataPositionFunc uPosition() const;
	DataPositionFunc vPosition() const;

	Vector2I resolution()const;
	double gridSpacing()const;

	Size2 uSize();
	Size2 vSize();

	Array2D& uDatas();
	Array2D& vDatas();

	Vector2D uOrigin() const;
	Vector2D vOrigin() const;

	Vector2D sample(const Vector2D& x) const;

private:
	Array2D _dataU;
	Array2D _dataV;
	Vector2D _dataOriginU;
	Vector2D _dataOriginV;
	BoundingBox2 _domain;
	Vector2I _resolution;
	Vector2D _gridSpacing;

	LinearArraySampler2<double> _uLinearSampler;
	LinearArraySampler2<double> _vLinearSampler;

	std::function<Vector2D(const Vector2D&)> _sampler;

};
typedef std::shared_ptr<MovingFaceCenteredGrid2> MovingFaceCenteredGrid2Ptr;


double& MovingFaceCenteredGrid2::u(int i, int j) {}
double& MovingFaceCenteredGrid2::v(int i, int j) {}

MovingFaceCenteredGrid2::DataPositionFunc MovingFaceCenteredGrid2::uPosition() const {}
MovingFaceCenteredGrid2::DataPositionFunc MovingFaceCenteredGrid2::vPosition() const {}

Vector2I MovingFaceCenteredGrid2::resolution()const {}
double MovingFaceCenteredGrid2::gridSpacing()const {}

Size2 MovingFaceCenteredGrid2::uSize() {}
Size2 MovingFaceCenteredGrid2::vSize() {}

Array2D& MovingFaceCenteredGrid2::uDatas() {}
Array2D& MovingFaceCenteredGrid2::vDatas() {}

Vector2D MovingFaceCenteredGrid2::uOrigin() const {}
Vector2D MovingFaceCenteredGrid2::vOrigin() const {}

Vector2D MovingFaceCenteredGrid2::sample(const Vector2D& x) const {}


#endif