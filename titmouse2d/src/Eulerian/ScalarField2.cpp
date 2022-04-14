#include "ScalarField2.h"

ScalarField2::ScalarField2() {

}


ScalarField2::~ScalarField2() {

}



Vector2D ScalarField2::gradient(const Vector2D& x)const {
	return Vector2D(0.0, 0.0);
}


double ScalarField2::laplacian(const Vector2D& x)const {
	return 0.0;
}


std::function<double(const Vector2D&)> ScalarField2::sampler()const {
	const ScalarField2* self = this;
	return [self](const Vector2D& x)->double {
		return self->sample(x);
	};
}
