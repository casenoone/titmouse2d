#ifndef MATRIX2X2_HPP
#define MATRIX2X2_HPP


#include <iostream>
using namespace std;

#include <array>

#include "Vector2.hpp"

//注意深浅拷贝的问题

template<class T>
class Matrix2x2 {
public:
	Matrix2x2();

	~Matrix2x2() {

	}

	Matrix2x2(T x1, T y1, T x2, T y2);

	T& operator()(size_t i, size_t j);

	T lookAt(size_t i, size_t j)const;

	Matrix2x2<T> operator+(const Matrix2x2<T>& mat)const;

	Matrix2x2<T> operator+=(const Matrix2x2<T>& mat);

	Matrix2x2<T> operator-(const Matrix2x2<T>& mat)const;

	Matrix2x2<T> operator*(T num)const;

	Matrix2x2<T> operator/(T num)const;

	template<class B>
	friend Matrix2x2<B> operator*(B num, const Matrix2x2<B>& mat);

	Vector2<T> operator*(const Vector2<T>& vec)const;

	T trace()const;

private:
	std::array<T, 4> _data;
};

template<class T>
Matrix2x2<T>::Matrix2x2() {
	_data[0] = 0;
	_data[1] = 0;
	_data[2] = 0;
	_data[3] = 0;
}

template<class T>
Matrix2x2<T>::Matrix2x2(T x1, T y1, T x2, T y2) {
	_data[0] = x1;
	_data[1] = y1;
	_data[2] = x2;
	_data[3] = y2;
}

template<class T>
T& Matrix2x2<T>::operator()(size_t i, size_t j) {
	return _data[2 * i + j];
}

template<class T>
T  Matrix2x2<T>::lookAt(size_t i, size_t j)const {
	return _data[2 * i + j];
}

template<class T>
Vector2<T> Matrix2x2<T>::operator*(const Vector2<T>& vec)const {
	auto x = _data[0] * vec.x + _data[1] * vec.y;
	auto y = _data[2] * vec.x + _data[3] * vec.y;
	return Vector2<T>(x, y);
}

template<class T>
Matrix2x2<T>  Matrix2x2<T>::operator*(T num)const {

	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		result._data[i] = _data[i] * num;
	}
	return result;
}

template<class T>
Matrix2x2<T>  Matrix2x2<T>::operator/(T num)const {

	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		result._data[i] = _data[i] / num;
	}
	return result;
}


template<class B>
Matrix2x2<B> operator*(B num, const Matrix2x2<B>& mat) {
	return mat * num;
}


template<class T>
Matrix2x2<T> Matrix2x2<T>::operator+(const Matrix2x2<T>& mat)const {
	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		result._data[i] = _data[i] + mat._data[i];
	}
	return result;
}


template<class T>
Matrix2x2<T> Matrix2x2<T>::operator-(const Matrix2x2<T>& mat)const {
	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		result._data[i] = _data[i] - mat._data[i];
	}
	return result;
}


template<class T>
Matrix2x2<T> Matrix2x2<T>::operator+=(const Matrix2x2<T>& mat) {
	for (int i = 0; i < 4; ++i) {
		_data[i] += mat._data[i];
	}
	return *this;
}

template<class T>
T Matrix2x2<T>::trace()const {
	return _data[0] + _data[3];
}


#endif