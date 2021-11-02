#ifndef SCALARGRID2_HPP
#define SCALARGRID2_HPP

#include "Grid2.hpp"
#include "ScalarField2.h"
#include "../Array2Ptr.hpp"

template<class T>
class ScalarGrid2 : public ScalarField2<T>, public Grid2<T> {
public:
	ScalarGrid2();

	virtual ~ScalarGrid2();

	virtual Vector2<T> dataSize()const = 0;
	
	virtual Vector2<T> dataOrigin()const = 0;

	virtual std::shared_ptr<ScalarGrid2> clone() const = 0;

	//返回给定下标的数据
	T& operator()(size_t i, size_t j);

	//返回给定点的梯度算子结果
	Vector2<T> gradientAtDataPoint(size_t i, size_t j) const;

	T laplacianAtDataPoint(size_t i, size_t j) const;
	
	typedef function<Vector2<T>(size_t, size_t)> DataPositionFunc;
	
	DataPositionFunc dataPosition() const;

	T sample(const Vector2<T>& x) const override;

	std::function<double(const Vector2<T>&)> sampler() const override;

	Vector2<T> gradient(const Vector2<T>& x) const override;

	T laplacian(const Vector2<T>& x) const override;

	void fill(double value);

	

protected:
	Array2Ptr<T> _data;

};

template<class T>
ScalarGrid2<T>::ScalarGrid2() {

}


template<class T>
ScalarGrid2<T>::~ScalarGrid2() {

}

template<class T>
T& ScalarGrid2<T>::operator()(size_t i, size_t j) {
	return _data(i, j);
}

template<class T>
Vector2<T> ScalarGrid2<T>::gradientAtDataPoint(size_t i, size_t j) const {

}


template<class T>
T ScalarGrid2<T>::laplacianAtDataPoint(size_t i, size_t j) const {

}

template<class T>

ScalarGrid2<T>::DataPositionFunc ScalarGrid2<T>::dataPosition() const {

}


template<class T>
T ScalarGrid2<T>::sample(const Vector2<T>& x) const  {

}


template<class T>
std::function<double(const Vector2<T>&)> ScalarGrid2<T>::sampler() const  {

}


template<class T>
Vector2<T> ScalarGrid2<T>::gradient(const Vector2<T>& x) const  {

}


template<class T>
T ScalarGrid2<T>::laplacian(const Vector2<T>& x) const  {

}


template<class T>
void ScalarGrid2<T>::fill(double value) {

}


template<class T>
class ScalarGridBuilder2 {
public:

	ScalarGridBuilder2();

	virtual ~ScalarGridBuilder2();

	virtual ScalarGrid2Ptr build(
		const Vector2<T>& resolution,
		const Vector2<T>& gridSpacing,
		const Vector2<T>& gridOrigin,
		double initialVal) const = 0;
};

template<class T>
ScalarGridBuilder2<T>::ScalarGridBuilder2() {

}


template<class T>
ScalarGridBuilder2<T>::~ScalarGridBuilder2() {

}

#endif