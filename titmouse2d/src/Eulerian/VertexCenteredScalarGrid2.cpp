#include "VertexCenteredScalarGrid2.h"

VertexCenteredScalarGrid2::VertexCenteredScalarGrid2() {
}


VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
	const Vector2I& resolution,
	const Vector2D& origin,
	double initialValue) {
	auto gridSpacing = Vector2D(2.0 / resolution.x, 2.0 / resolution.y);

	std::vector<std::vector<double>> temp;

	resize(resolution, gridSpacing, origin, initialValue);
	auto size = resolution + Vector2I(1, 1);
	_data.reSize(size.x, size.y, initialValue);

	this->_linearSampler = std::make_shared<LinearArraySampler2<double>>(this->_data, gridSpacing, dataOrigin());

}


void VertexCenteredScalarGrid2::clearData(double initialValue) {
	auto res = resolution();
	auto newres = res + Vector2I(1, 1);
	_data.reSize(newres.x, newres.y, initialValue);
}


//暂时不实现
VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
	const VertexCenteredScalarGrid2& other) {
	(*this) = other;
}



Vector2I VertexCenteredScalarGrid2::dataSize() const {
	auto tempV = resolution();
	if (tempV.x != 0.0 && tempV.y != 0.0) {
		return resolution() + Vector2I(1, 1);
	}
	else {
		return Vector2I(0, 0);
	}
}

Vector2D VertexCenteredScalarGrid2::dataOrigin() const {
	return origin();
}




VertexCenteredScalarGrid2::Builder& VertexCenteredScalarGrid2::Builder::withResolution(const Vector2I& resolution) {
	_resolution = resolution;
	return *this;
}

VertexCenteredScalarGrid2::Builder& VertexCenteredScalarGrid2::Builder::withResolution(
	int resolutionX, int resolutionY) {
	_resolution.x = resolutionX;
	_resolution.y = resolutionY;
	return *this;
}




VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::withOrigin(const Vector2D& gridOrigin) {
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
			_gridOrigin,
			_initialVal),
		[](VertexCenteredScalarGrid2* obj) {
			delete obj;
		});
}


VertexCenteredScalarGrid2 VertexCenteredScalarGrid2::Builder::build() const {

	return VertexCenteredScalarGrid2(
		_resolution,
		_gridOrigin,
		_initialVal);

}

ScalarGrid2Ptr VertexCenteredScalarGrid2::Builder::build(
	const Vector2I& resolution,
	const Vector2D& gridOrigin,
	double initialVal) const {
	return std::shared_ptr<VertexCenteredScalarGrid2>(
		new VertexCenteredScalarGrid2(
			resolution,
			gridOrigin,
			initialVal),
		[](VertexCenteredScalarGrid2* obj) {
			delete obj;
		});
}


VertexCenteredScalarGrid2::Builder VertexCenteredScalarGrid2::builder() {
	return Builder();
}
