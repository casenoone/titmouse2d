#ifndef ARRAYSAMPLER2_HPP
#define ARRAYSAMPLER2_HPP

#include "../Vector2.hpp"
#include <functional>

#include "../Array2.hpp"
#include "../MathUtils.hpp"

#include "../ConstVar.h"

#include <iostream>

//线性采样器
//需要实现一个更高阶的插值技术
//例如BFECC方法




template<class T>
class LinearArraySampler2 {
public:

	LinearArraySampler2();

	~LinearArraySampler2();

	LinearArraySampler2(
		const Array2<T>& accessor,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	LinearArraySampler2(
		Array2<T>& accessor,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);


	LinearArraySampler2(const LinearArraySampler2& other);

	void resize(
		const Array2<T>& accessor,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	T operator()(const Array2<T>& accessor, const Vector2D& x)const;

	void getCoordinatesAndGradientWeights(
		const Vector2D& x,
		std::array<Vector2I, 4>* indices,
		std::array<Vector2D, 4>* weights)const;

	void getCoordinatesAndWeights(
		const Vector2D& x,
		std::array<Vector2I, 4>* indices,
		std::array<double, 4>* weights);

	//这里需要改一下接口,这里本应该是私有成员
	//由于设计不当，这里不得不暂时改为公有
public:
	Array2<T> _accessor;

private:
	Vector2D _gridSpacing;
	Vector2D _invGridSpacing;
	Vector2D _origin;

};

template<typename T>
using LinearArraySampler2Ptr = std::shared_ptr<LinearArraySampler2<T>>;




template<class T>
LinearArraySampler2<T>::LinearArraySampler2() {

}

template<class T>
LinearArraySampler2<T>::~LinearArraySampler2() {

}



template<class T>
LinearArraySampler2<T>::LinearArraySampler2(
	const Array2<T>& accessor,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) {
	_gridSpacing = gridSpacing;
	_invGridSpacing = Vector2D(1 / gridSpacing.x, 1 / gridSpacing.y);

	_origin = gridOrigin;
	_accessor.set(accessor);
	//cout << "深拷贝" << endl;
}

template<class T>
LinearArraySampler2<T>::LinearArraySampler2(
	Array2<T>& accessor,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) {
	_gridSpacing = gridSpacing;
	_invGridSpacing = Vector2D(1 / gridSpacing.x, 1 / gridSpacing.y);

	_origin = gridOrigin;
	_accessor.shallowCopy(accessor);
	//_accessor = accessor;
	//cout << "浅拷贝" << endl;
}

template<class T>
void LinearArraySampler2<T>::resize(
	const Array2<T>& accessor,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) {
	_gridSpacing = gridSpacing;
	_invGridSpacing = Vector2D(1 / gridSpacing.x, 1 / gridSpacing.y);

	_origin = gridOrigin;
	_accessor.set(accessor);
}

template<class T>
LinearArraySampler2<T>::LinearArraySampler2(const LinearArraySampler2<T>& other) {

	_gridSpacing = other._gridSpacing;
	_invGridSpacing = other._invGridSpacing;
	_origin = other._origin;
	_accessor = other._accessor;

}


template<class T>
T LinearArraySampler2<T>::operator()(const Array2<T>& accessor, const Vector2D& x) const {
	int i, j;
	double fx, fy;
	Vector2D normalizedX = (x - _origin) / _gridSpacing;
	auto& accessors = const_cast<Array2<T>&>(accessor);
	auto size = accessors.dataSize();
	int iSize = size.x;
	int jSize = size.y;


	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);


	//双线性插值
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
	const Vector2D& x,
	std::array<Vector2I, 4>* indices,
	std::array<Vector2D, 4>* weights) const {
	int i, j;
	double fx, fy;


	const Vector2D normalizedX = (x - _origin) * _invGridSpacing;

	auto size = _accessor.dataSize();

	int iSize = size.x;
	int jSize = size.y;

	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);

	(*indices)[0] = Vector2I(i, j);
	(*indices)[1] = Vector2I(ip1, j);
	(*indices)[2] = Vector2I(i, jp1);
	(*indices)[3] = Vector2I(ip1, jp1);

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


//计算粒子在各个节点上的插值
template<class T>
void LinearArraySampler2<T>::getCoordinatesAndWeights(
	const Vector2D& x,
	std::array<Vector2I, 4>* indices,
	std::array<double, 4>* weights) {

	//这步操作在做这样一个事情：
	//把粒子坐标映射到某个网格单元的局部坐标，然后将新坐标单位化
	//为的是保证权重之和为1


	//这里的origin指的是背景网格数据点的起点，而非每一个小cell里的起点


	Vector2D normalizedX = (x - _origin) / _gridSpacing;

	auto size = _accessor.dataSize();

	int iSize = size.x;
	int jSize = size.y;

	int i, j;
	double fx, fy;

	//得到粒子所在的格子的下标以及粒子在格子中的相对坐标
	getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
	getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

	//i，j实际上就是粒子所在的格子的索引
	//要求一个cell中四个节点的索引
	int ip1 = std::min(i + 1, iSize - 1);
	int jp1 = std::min(j + 1, jSize - 1);

	(*indices)[0] = Vector2I(i, j);
	(*indices)[1] = Vector2I(ip1, j);
	(*indices)[2] = Vector2I(i, jp1);
	(*indices)[3] = Vector2I(ip1, jp1);



	//双线性插值
	(*weights)[0] = (1 - fx) * (1 - fy);
	(*weights)[1] = fx * (1 - fy);
	(*weights)[2] = (1 - fx) * fy;
	(*weights)[3] = fx * fy;

}






#endif