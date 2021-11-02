#ifndef SCALARFIELD2_HPP
#define SCALARFIELD2_HPP

#include "Field2.hpp"
#include "../Vector2.hpp"

#include<functional>

template<class T>
class ScalarField2 : public Field2 {
public:
	ScalarField2();

	virtual ~ScalarField2();

	//在给定点采样值
	virtual T sample(const Vector2<T>& x)const = 0;

	//算梯度
	virtual Vector2<T> gradient(const Vector2<T>& x)const;

	//算拉普拉斯
	virtual T laplacian(const Vector2<T>& x)const;

	virtual function<T(const Vector2<T>&)> sampler()const;
};

template<class T>
ScalarField2<T>::ScalarField2() {

}


template<class T>
ScalarField2<T>::~ScalarField2() {

}



template<class T>
Vector2<T> ScalarField2<T>::gradient(const Vector2<T>& x)const {
	return Vector2<T>(0, 0);
}


template<class T>
T ScalarField2<T>::laplacian(const Vector2<T>& x)const {
	return 0.0;
}


template<class T>
function<T(const Vector2<T>&)> ScalarField2<T>::sampler()const {
	const ScalarField2* self = this;
	return [self](const Vector2& x)->double {
		return self->sample(x);
	};
}


#endif