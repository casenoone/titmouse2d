#include "ScalarGrid2.h"

ScalarGrid2::ScalarGrid2() {

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


//暂时不实现
//ScalarGrid2::DataPositionFunc ScalarGrid2::dataPosition() const {
//
//}


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