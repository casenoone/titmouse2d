#ifndef MATRIX2X2_HPP
#define MATRIX2X2_HPP


#include <iostream>

#include <array>

#include "Vector2.hpp"

//注意深浅拷贝的问题
//这个类是一个浅拷贝类，但拷贝复制运算符是一个深拷贝
//实际上实现一个set或copy是更好的做法，而不要重载等于号

template<class T>
class Matrix2x2 {
public:
	Matrix2x2();

	~Matrix2x2() {

	}

	Matrix2x2(T x1, T y1, T x2, T y2);

	Matrix2x2(const Matrix2x2<T>& m);

	T& operator()(int i, int j);

	T lookAt(int i, int j)const;

	//实现类的深拷贝
	Matrix2x2<T>& operator=(const Matrix2x2<T>& mat);

	Matrix2x2<T> operator+(const Matrix2x2<T>& mat)const;

	Matrix2x2<T>& operator+=(const Matrix2x2<T>& mat);

	Matrix2x2<T> operator-(const Matrix2x2<T>& mat)const;

	Matrix2x2<T> operator*(T num)const;

	Matrix2x2<T> operator*(const Matrix2x2<T>& mat)const;

	Matrix2x2<T> operator/(T num)const;

	Matrix2x2<T>& operator*=(T num);

	template<class B>
	friend Matrix2x2<B> operator*(B num, const Matrix2x2<B>& mat);

	Vector2<T> operator*(const Vector2<T>& vec)const;

	Matrix2x2<T> inverse();

	T trace()const;

	Matrix2x2<T> transpose()const;


	static Matrix2x2<T> identityMatrix() {
		Matrix2x2<T> result(1, 0, 0, 1);
		return result;
	}

	static Matrix2x2<T> rotationMatrix(double radian) {
		auto cos_ = std::cos(radian);
		auto sin_ = std::sin(radian);
		return { cos_,-sin_,sin_,cos_ };
	}


private:
	std::shared_ptr<std::array<T, 4>> _data;
};


template<class T>
Matrix2x2<T>::Matrix2x2() {

	_data = std::make_shared<std::array<T, 4>>();

	(*_data)[0] = 1;
	(*_data)[1] = 0;
	(*_data)[2] = 0;
	(*_data)[3] = 1;
}


//浅拷贝
template<class T>
Matrix2x2<T>::Matrix2x2(const Matrix2x2<T>& m) {
	_data = m._data;
}


template<class T>
Matrix2x2<T>::Matrix2x2(T x1, T y1, T x2, T y2) {

	_data = std::make_shared<std::array<T, 4>>();

	(*_data)[0] = x1;
	(*_data)[1] = y1;
	(*_data)[2] = x2;
	(*_data)[3] = y2;
}

template<class T>
T& Matrix2x2<T>::operator()(int i, int j) {
	return (*_data)[2 * i + j];
}

template<class T>
T  Matrix2x2<T>::lookAt(int i, int j)const {
	return (*_data)[2 * i + j];
}

template<class T>
Vector2<T> Matrix2x2<T>::operator*(const Vector2<T>& vec)const {
	auto x = (*_data)[0] * vec.x + (*_data)[1] * vec.y;
	auto y = (*_data)[2] * vec.x + (*_data)[3] * vec.y;
	return Vector2<T>(x, y);
}

template<class T>
Matrix2x2<T>  Matrix2x2<T>::operator*(T num)const {

	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		(*result._data)[i] = (*_data)[i] * num;
	}
	return result;
}

template<class T>
Matrix2x2<T> Matrix2x2<T>::operator*(const Matrix2x2<T>& mat)const {
	auto x11 = (*_data)[0] * mat.lookAt(0, 0) + (*_data)[1] * mat.lookAt(1, 0);
	auto x12 = (*_data)[0] * mat.lookAt(0, 1) + (*_data)[1] * mat.lookAt(1, 1);
	auto x21 = (*_data)[2] * mat.lookAt(0, 0) + (*_data)[3] * mat.lookAt(1, 0);
	auto x22 = (*_data)[2] * mat.lookAt(0, 1) + (*_data)[3] * mat.lookAt(1, 1);

	Matrix2x2<T> result(x11, x12, x21, x22);
	return result;
}


template<class T>
Matrix2x2<T>  Matrix2x2<T>::operator/(T num)const {

	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		(*result._data)[i] = (*_data)[i] / num;
	}
	return result;
}


template<class B>
Matrix2x2<B> operator*(B num, const Matrix2x2<B>& mat) {
	return mat * num;
}

template<class T>
Matrix2x2<T>& Matrix2x2<T>::operator=(const Matrix2x2<T>& mat) {
	(*_data) = (*mat._data);
	return *this;
}

template<class T>
Matrix2x2<T> Matrix2x2<T>::operator+(const Matrix2x2<T>& mat)const {
	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		(*result._data)[i] = (*_data)[i] + (*mat._data)[i];
	}
	return result;
}


template<class T>
Matrix2x2<T> Matrix2x2<T>::operator-(const Matrix2x2<T>& mat)const {
	Matrix2x2<T> result;
	for (int i = 0; i < 4; ++i) {
		(*result._data)[i] = (*_data)[i] - (*mat._data)[i];
	}
	return result;
}


template<class T>
Matrix2x2<T>& Matrix2x2<T>::operator+=(const Matrix2x2<T>& mat) {
	for (int i = 0; i < 4; ++i) {
		(*_data)[i] += (*mat._data)[i];
	}
	return *this;
}


template<class T>
Matrix2x2<T>& Matrix2x2<T>::operator*=(T num) {
	for (int i = 0; i < 4; ++i) {
		(*_data)[i] *= num;
	}
	return *this;
}

template<class T>
Matrix2x2<T> Matrix2x2<T>::inverse() {
	auto a = (*_data)[0];
	auto b = (*_data)[1];
	auto c = (*_data)[2];
	auto d = (*_data)[3];
	Matrix2x2<T> result(d, -b, -c, a);
	auto k = 1 / (a * d - b * c);
	result *= k;
	return result;
}



template<class T>
T Matrix2x2<T>::trace()const {
	return (*_data)[0] + (*_data)[3];
}

template<class T>
Matrix2x2<T>  Matrix2x2<T>::transpose()const {
	auto a11 = (*_data)[0];
	auto a12 = (*_data)[2];
	auto a21 = (*_data)[1];
	auto a22 = (*_data)[3];

	Matrix2x2<double> result(a11, a12, a21, a22);
	return result;
}


#endif