#ifndef SCALARFIELD2_HPP
#define SCALARFIELD2_HPP

#include "Field2.h"
#include "../Vector2.hpp"

#include<functional>

class ScalarField2 : public Field2 {
public:
	ScalarField2();

	virtual ~ScalarField2();

	//在给定点采样值

	virtual double sample(const Vector2<double>& x)const = 0;

	//算梯度
	virtual Vector2<double> gradient(const Vector2<double>& x)const;

	//算拉普拉斯
	virtual double laplacian(const Vector2<double>& x)const;

	virtual function<double(const Vector2<double>&)> sampler()const;
};



#endif