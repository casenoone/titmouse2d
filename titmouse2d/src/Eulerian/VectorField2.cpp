#include "VectorField2.h"

VectorField2::VectorField2() {
}

VectorField2::~VectorField2() {
}

double VectorField2::divergence(const Vector2<double>&) const {
    return 0.0;
}

double VectorField2::curl(const Vector2<double>&) const {
    return 0.0;
}

std::function<Vector2<double>(const Vector2<double>&)> VectorField2::sampler() const {
    const VectorField2* self = this;
    return [self](const Vector2<double>& x) -> Vector2<double> {
        return self->sample(x);
    };
}
