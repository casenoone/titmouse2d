#ifndef ARRAYSAMPLER2_HPP
#define ARRAYSAMPLER2_HPP

#include "../Vector2.hpp"
#include <functional>

#include "../Array2Ptr.hpp"
#include "../MathUtils.hpp"

#include "../ConstVar.h"

#include <iostream>

//���Բ�����
//��Ҫʵ��һ�����߽׵Ĳ�ֵ����
//����BFECC����




template<class T>
class LinearArraySampler2 {
public:

	LinearArraySampler2();

	~LinearArraySampler2();

	LinearArraySampler2(
		const Array2Ptr<T>& accessor,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	LinearArraySampler2(const LinearArraySampler2& other);

	T operator()(const Array2Ptr<T>& accessor, const Vector2<double>& x);

	void getCoordinatesAndGradientWeights(
		const Vector2<double>& x,
		std::array<Vector2<int>, 4>* indices,
		std::array<Vector2<double>, 4>* weights)const;

	void getCoordinatesAndWeights(
		const Vector2<double>& x,
		std::array<Vector2<int>, 4>* indices,
		std::array<double, 4>* weights);

	//������Ҫ��һ�½ӿ�,���ﱾӦ����˽�г�Ա
	//������Ʋ��������ﲻ�ò���ʱ��Ϊ����
public:
	Array2Ptr<T> _accessor;

private:
	Vector2<double> _gridSpacing;
	Vector2<double> _invGridSpacing;
	Vector2<double> _origin;

};

template<typename T>
using LinearArraySampler2Ptr = shared_ptr<LinearArraySampler2<T>>;




template<class T>
LinearArraySampler2<T>::LinearArraySampler2() {

}

template<class T>
LinearArraySampler2<T>::~LinearArraySampler2() {

}



template<class T>
LinearArraySampler2<T>::LinearArraySampler2(
	const Array2Ptr<T>& accessor,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin) {
	_gridSpacing = gridSpacing;
	_invGridSpacing = Vector2<double>(1 / gridSpacing.x, 1 / gridSpacing.y);

	_origin = gridOrigin;
	_accessor = accessor;

}

template<class T>
LinearArraySampler2<T>::LinearArraySampler2(const LinearArraySampler2<T>& other) {

	_gridSpacing = other._gridSpacing;
	_invGridSpacing = other._invGridSpacing;
	_origin = other._origin;
	_accessor = other._accessor;

}


template<class T>
T LinearArraySampler2<T>::operator()(const Array2Ptr<T>& accessor, const Vector2<double>& x) {
	int i, j;
	double fx, fy;
	Vector2<double> normalizedX = (x - _origin) / _gridSpacing;
	auto& accessors = const_cast<Array2Ptr<T>&>(accessor);
	auto size = accessors.dataSize();
	int iSize = size.x;
	int jSize = size.y;

	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);


	////˫���Բ�ֵ
	return bilerp(
		accessors(i, j),
		accessors(ip1, j),
		accessors(i, jp1),
		accessors(ip1, jp1),
		fx,
		fy);

}


template <class T>
void LinearArraySampler2<T>::getCoordinatesAndGradientWeights(
	const Vector2<double>& x,
	std::array<Vector2<int>, 4>* indices,
	std::array<Vector2<double>, 4>* weights) const {
	int i, j;
	double fx, fy;


	const Vector2<double> normalizedX = (x - _origin) * _invGridSpacing;

	auto size = _accessor.dataSize();

	int iSize = size.x;
	int jSize = size.y;

	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);

	(*indices)[0] = Vector2<int>(i, j);
	(*indices)[1] = Vector2<int>(ip1, j);
	(*indices)[2] = Vector2<int>(i, jp1);
	(*indices)[3] = Vector2<int>(ip1, jp1);

	(*weights)[0] = Vector2<T>(
		fy * _invGridSpacing.x - _invGridSpacing.x,
		fx * _invGridSpacing.y - _invGridSpacing.y);
	(*weights)[1] = Vector2<T>(
		-fy * _invGridSpacing.x + _invGridSpacing.x,
		-fx * _invGridSpacing.y);
	(*weights)[2] = Vector2<T>(
		-fy * _invGridSpacing.x,
		-fx * _invGridSpacing.y + _invGridSpacing.y);
	(*weights)[3] = Vector2<T>(
		fy * _invGridSpacing.x,
		fx * _invGridSpacing.y);
}


//���������ڸ����ڵ��ϵĲ�ֵ
template<class T>
void LinearArraySampler2<T>::getCoordinatesAndWeights(
	const Vector2<double>& x,
	std::array<Vector2<int>, 4>* indices,
	std::array<double, 4>* weights) {

	//�ⲽ������������һ�����飺
	//����������ӳ�䵽ĳ������Ԫ�ľֲ����꣬Ȼ�������굥λ��
	//Ϊ���Ǳ�֤Ȩ��֮��Ϊ1


	//�����originָ���Ǳ����������ݵ����㣬����ÿһ��Сcell������


	Vector2<double> normalizedX = (x - _origin) / _gridSpacing;

	auto size = _accessor.dataSize();

	int iSize = size.x;
	int jSize = size.y;

	int i, j;
	double fx, fy;

	//�õ��������ڵĸ��ӵ��±��Լ������ڸ����е��������
	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

	//i��jʵ���Ͼ����������ڵĸ��ӵ�����
	//Ҫ��һ��cell���ĸ��ڵ������
	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);

	(*indices)[0] = Vector2<int>(i, j);
	(*indices)[1] = Vector2<int>(ip1, j);
	(*indices)[2] = Vector2<int>(i, jp1);
	(*indices)[3] = Vector2<int>(ip1, jp1);

	//˫���Բ�ֵ
	(*weights)[0] = (1 - fx) * (1 - fy);
	(*weights)[1] = fx * (1 - fy);
	(*weights)[2] = (1 - fx) * fy;
	(*weights)[3] = fx * fy;

}






#endif