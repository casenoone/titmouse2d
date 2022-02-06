#include "CellCenteredScalarGrid2.h"


CellCenteredScalarGrid2::CellCenteredScalarGrid2() {

}



CellCenteredScalarGrid2::~CellCenteredScalarGrid2() {

}




CellCenteredScalarGrid2::CellCenteredScalarGrid2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& origin,
	double initialValue) {

	auto newgridspacing = Vector2<double>(2.0 / resolution.x, 2.0 / resolution.y);
	this->resize(resolution, newgridspacing, origin, initialValue);
	this->_linearSampler = make_shared<LinearArraySampler2<double>>(this->_data, newgridspacing, dataOrigin());

}


Vector2<size_t> CellCenteredScalarGrid2::dataSize() const {
	return this->resolution();
}


Vector2<double> CellCenteredScalarGrid2::dataOrigin() const {
	return this->origin() + this->gridSpacing() * 0.5;
}



CellCenteredScalarGrid2::Builder CellCenteredScalarGrid2::builder() {
	return Builder();
}


CellCenteredScalarGrid2::Builder&
CellCenteredScalarGrid2::Builder::withResolution(const Vector2<size_t>& resolution) {
	_resolution = resolution;
	return *this;
}


CellCenteredScalarGrid2::Builder&
CellCenteredScalarGrid2::Builder::withResolution(
	size_t resolutionX, size_t resolutionY) {
	_resolution.x = resolutionX;
	_resolution.y = resolutionY;
	return *this;
}



CellCenteredScalarGrid2::Builder&
CellCenteredScalarGrid2::Builder::withOrigin(const Vector2<double>& gridOrigin) {
	_gridOrigin = gridOrigin;
	return *this;
}


CellCenteredScalarGrid2::Builder&
CellCenteredScalarGrid2::Builder::withOrigin(
	double gridOriginX, double gridOriginY) {
	_gridOrigin.x = gridOriginX;
	_gridOrigin.y = gridOriginY;
	return *this;
}


CellCenteredScalarGrid2::Builder&
CellCenteredScalarGrid2::Builder::withInitialValue(double initialVal) {
	_initialVal = initialVal;
	return *this;
}


CellCenteredScalarGrid2 CellCenteredScalarGrid2::Builder::build() const {
	return CellCenteredScalarGrid2(
		_resolution,
		_gridOrigin,
		_initialVal);
}


ScalarGrid2Ptr CellCenteredScalarGrid2::Builder::build(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridOrigin,
	double initialVal) const {
	return std::shared_ptr<CellCenteredScalarGrid2>(
		new CellCenteredScalarGrid2(
			resolution,
			gridOrigin,
			initialVal),
		[](CellCenteredScalarGrid2* obj) {
			delete obj;
		});
}


CellCenteredScalarGrid2Ptr CellCenteredScalarGrid2::Builder::makeShared() const {
	return std::shared_ptr<CellCenteredScalarGrid2>(
		new CellCenteredScalarGrid2(
			_resolution,
			_gridOrigin,
			_initialVal),
		[](CellCenteredScalarGrid2* obj) {
			delete obj;
		});
}
