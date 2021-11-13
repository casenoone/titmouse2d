#ifndef VECTORFIELD2_H
#define VECTORFIELD2_H

#include "Field2.h"
#include "../Vector2.hpp"
#include <functional>
#include <iostream>

class VectorField2 :public Field2 {
public:
	VectorField2();
	virtual ~VectorField2();
	virtual Vector2<double> sample(const Vector2<double>& x) const = 0;
	virtual double divergence(const Vector2<double>& x) const;
	virtual double curl(const Vector2<double>& x) const;

	virtual std::function<Vector2<double>(const Vector2<double>&)> sampler() const;
};

typedef std::shared_ptr<VectorField2> VectorField2Ptr;




#endif