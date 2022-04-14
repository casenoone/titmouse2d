#ifndef SCALARFIELD2_HPP
#define SCALARFIELD2_HPP

#include "Field2.h"
#include "../Vector2.hpp"

#include<functional>

class ScalarField2 : public Field2 {
public:
	ScalarField2();

	virtual ~ScalarField2();

	//�ڸ��������ֵ

	virtual double sample(const Vector2D& x)const = 0;

	//���ݶ�
	virtual Vector2D gradient(const Vector2D& x)const;

	//��������˹
	virtual double laplacian(const Vector2D& x)const;

	virtual std::function<double(const Vector2D&)> sampler()const;
};



#endif