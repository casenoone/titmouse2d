#include "ScalarField2.h"

ScalarField2::ScalarField2() {

}


ScalarField2::~ScalarField2() {

}



Vector2<double> ScalarField2::gradient(const Vector2<double>& x)const {
	return Vector2<double>(0.0, 0.0);
}


double ScalarField2::laplacian(const Vector2<double>& x)const {
	return 0.0;
}


function<double(const Vector2<double>&)> ScalarField2::sampler()const {
	const ScalarField2* self = this;
	return [self](const Vector2<double>& x)->double {
		return self->sample(x);
	};
}
