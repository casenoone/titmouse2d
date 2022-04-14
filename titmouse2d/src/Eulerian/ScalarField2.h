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

	virtual double sample(const Vector2D& x)const = 0;

	//算梯度
	virtual Vector2D gradient(const Vector2D& x)const;

	//算拉普拉斯
	virtual double laplacian(const Vector2D& x)const;

	virtual std::function<double(const Vector2D&)> sampler()const;
};



#endif