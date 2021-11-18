#include "ScalarGrid2.h"

ScalarGrid2::ScalarGrid2():_data(vector<vector<double>>()) {

}


ScalarGrid2::~ScalarGrid2() {

}

double& ScalarGrid2::operator()(size_t i, size_t j) {
	return _data(i, j);
}

Vector2<double> ScalarGrid2::gradientAtDataPoint(size_t i, size_t j) const {
	return Vector2<double>(0.0, 0.0);
}


double ScalarGrid2::laplacianAtDataPoint(size_t i, size_t j) const {
	return 0.0;
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

//��ʱ��ʵ��
ScalarGrid2::DataPositionFunc ScalarGrid2::dataPosition() const {
	Vector2<double> o = dataOrigin();
	//��size_tת����const double &��Ҫ����ת��
	return [this, o](double i, double j) -> Vector2<double> {
		return o + gridSpacing() * Vector2<double>({ i, j });
	};
}


double ScalarGrid2::sample(const Vector2<double>& x) const {
	return 0.0;
}


std::function<double(const Vector2<double>&)> ScalarGrid2::sampler() const {
	std::function<double(const Vector2<double>&)> t;
	return t;
}


Vector2<double> ScalarGrid2::gradient(const Vector2<double>& x) const {
	return Vector2<double>(0.0, 0.0);
}


double ScalarGrid2::laplacian(const Vector2<double>& x) const {
	return 0.0;
}


void ScalarGrid2::fill(double value) {

}


ScalarGridBuilder2::ScalarGridBuilder2() {

}


ScalarGridBuilder2::~ScalarGridBuilder2() {

}