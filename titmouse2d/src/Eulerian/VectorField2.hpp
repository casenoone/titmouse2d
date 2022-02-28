#ifndef VECTORFIELD2_HPP
#define VECTORFIELD2_HPP

#include "Field2.h"
#include "../Vector2.hpp"
#include <functional>
#include <iostream>

template<class T>
class VectorField2 :public Field2 {
public:
	VectorField2();
	virtual ~VectorField2();
	virtual T sample(const Vector2D& x) const = 0;
	virtual double divergence(const Vector2D& x) const;
	virtual double curl(const Vector2D& x) const;

	virtual std::function<T(const Vector2D&)> sampler() const;
};



template<class T>
VectorField2<T>::VectorField2() {
}

template<class T>
VectorField2<T>::~VectorField2() {
}

template<class T>
double VectorField2<T>::divergence(const Vector2D&) const {
	return 0.0;
}

template<class T>
double VectorField2<T>::curl(const Vector2D&) const {
	return 0.0;
}

template<class T>
std::function<T(const Vector2D&)> VectorField2<T>::sampler() const {
	const VectorField2<T>* self = this;
	return [self](const Vector2D& x) -> T {
		return self->sample(x);
	};
}


#endif