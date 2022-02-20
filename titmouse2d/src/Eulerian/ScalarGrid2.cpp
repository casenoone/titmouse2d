#include "ScalarGrid2.h"

#include <array>

ScalarGrid2::ScalarGrid2() :_data(vector<vector<double>>()) {

}


ScalarGrid2::~ScalarGrid2() {

}

double& ScalarGrid2::operator()(size_t i, size_t j) {
	return _data(i, j);
}

double ScalarGrid2::lookAt(size_t i, size_t j)const {
	return _data.lookAt(i, j);
}


double ScalarGrid2::sample(const Vector2<double>& x)const {
	auto _sample = _linearSampler;
	auto result = (*_sample)(this->_data, x);

	return result;
}


Vector2<double> ScalarGrid2::gradientAtDataPoint(size_t i, size_t j) const {
	const auto ds = _data.dataSize();

	double left = _data.lookAt((i > 0) ? i - 1 : i, j);
	double right = _data.lookAt((i + 1 < ds.x) ? i + 1 : i, j);
	double down = _data.lookAt(i, (j > 0) ? j - 1 : j);
	double up = _data.lookAt(i, (j + 1 < ds.y) ? j + 1 : j);

	return 0.5 * Vector2D(right - left, up - down)
		/ gridSpacing();

}


double ScalarGrid2::laplacianAtDataPoint(size_t i, size_t j) const {
	return 0.0;
}

Array2Ptr<double>& ScalarGrid2::datas() {
	return _data;
}


const Array2Ptr<double> ScalarGrid2::datas() const {
	return _data;
}


void ScalarGrid2::resize(const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& origin,
	double initialValue) {

	_resolution = resolution;
	_gridSpacing = gridSpacing;
	_origin = origin;

	auto size = dataSize();

	_data.reSize(size.x, size.y, initialValue);


}


void ScalarGrid2::clearData(double initialValue) {
	auto res = resolution();
	_data.reSize(res.x, res.y, initialValue);
}


ScalarGrid2::DataPositionFunc ScalarGrid2::dataPosition() const {
	Vector2<double> o = dataOrigin();
	//从size_t转换到const double &需要收缩转换
	return [this, o](double i, double j) -> Vector2<double> {
		return o + gridSpacing() * Vector2<double>({ i, j });
	};
}





std::function<double(const Vector2<double>&)> ScalarGrid2::sampler() const {
	std::function<double(const Vector2<double>&)> t;
	return t;
}


Vector2<double> ScalarGrid2::gradient(const Vector2<double>& x) const {

	auto sampler = _linearSampler;

	std::array<Vector2I, 4> indices;
	std::array<double, 4> weights;

	sampler->getCoordinatesAndWeights(x, &indices, &weights);

	Vector2D result;

	for (int i = 0; i < 4; ++i) {
		result += weights[i] * gradientAtDataPoint(indices[i].x, indices[i].y);
	}
	return result;
}


double ScalarGrid2::laplacian(const Vector2<double>& x) const {
	return 0.0;
}


void ScalarGrid2::fill(double value) {

	auto size = dataSize();
	for (size_t i = 0; i < size.x; ++i) {
		for (size_t j = 0; j < size.y; ++j) {
			_data(i, j) = value;
		}
	}

}


ScalarGridBuilder2::ScalarGridBuilder2() {

}


ScalarGridBuilder2::~ScalarGridBuilder2() {

}