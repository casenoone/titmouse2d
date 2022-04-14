#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP


#include <iostream>

#include <array>

#include "Vector2.hpp"
#include "Vector3.hpp"


template<class T>
class Matrix3x3 {
public:
	Matrix3x3();

	~Matrix3x3() {

	}

	//默认按行优先吧
	Matrix3x3(const Vector3<T>& vec1,
		const Vector3<T>& vec2,
		const Vector3<T>& vec3);

	Matrix3x3(const Matrix3x3<T>& m);

	T& operator()(int i, int j);

	T lookAt(int i, int j)const;

	//实现类的深拷贝
	Matrix3x3<T>& operator=(const Matrix3x3<T>& mat);

	Matrix3x3<T> operator+(const Matrix3x3<T>& mat)const;

	Matrix3x3<T>& operator+=(const Matrix3x3<T>& mat);

	Matrix3x3<T> operator-(const Matrix3x3<T>& mat)const;

	Matrix3x3<T> operator*(T num)const;

	Matrix3x3<T> operator*(const Matrix3x3<T>& mat)const;

	Matrix3x3<T> operator/(T num)const;

	Matrix3x3<T>& operator*=(T num);

	template<class B>
	friend Matrix3x3<B> operator*(B num, const Matrix3x3<B>& mat);

	Vector3<T> operator*(const Vector3<T>& vec)const;

	//重载一个3x3矩阵与二维向量的乘法，省的还得写齐次坐标
	Vector2<T> operator*(const Vector2<T>& vec)const;

	//把世界坐标系下的表示变换到局部坐标系下
	static Matrix3x3<T> transToLocalMatrix(
		const Vector2<T>& localY,
		const Vector2<T>& localO);

	Matrix3x3<T> inverse();

	T trace()const;

	T det()const;

private:
	std::shared_ptr<std::array<T, 9>> _data;
};


template<class T>
Matrix3x3<T>::Matrix3x3() {

	_data = std::make_shared<std::array<T, 9>>();

	for (int i = 0; i < 9; ++i) {
		(*_data)[i] = 0;
	}
}


//浅拷贝
template<class T>
Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& m) {
	_data = m._data;
}


template<class T>
Matrix3x3<T>::Matrix3x3(
	const Vector3<T>& vec1,
	const Vector3<T>& vec2,
	const Vector3<T>& vec3) {

	_data = std::make_shared<std::array<T, 9>>();

	(*_data)[0] = vec1.x;
	(*_data)[1] = vec1.y;
	(*_data)[2] = vec1.z;
	(*_data)[3] = vec2.x;
	(*_data)[4] = vec2.y;
	(*_data)[5] = vec2.z;
	(*_data)[6] = vec3.x;
	(*_data)[7] = vec3.y;
	(*_data)[8] = vec3.z;
}

template<class T>
T& Matrix3x3<T>::operator()(int i, int j) {
	return (*_data)[3 * i + j];
}

template<class T>
T  Matrix3x3<T>::lookAt(int i, int j)const {
	return (*_data)[3 * i + j];
}



template<class T>
Vector3<T> Matrix3x3<T>::operator*(const Vector3<T>& vec)const {
	auto x = (*_data)[0] * vec.x + (*_data)[1] * vec.y + (*_data)[2] * vec.z;
	auto y = (*_data)[3] * vec.x + (*_data)[4] * vec.y + (*_data)[5] * vec.z;
	auto z = (*_data)[6] * vec.x + (*_data)[7] * vec.y + (*_data)[8] * vec.z;
	return Vector3<T>(x, y, z);
}

template<class T>
Vector2<T> Matrix3x3<T>::operator*(const Vector2<T>& vec)const {
	auto x = (*_data)[0] * vec.x + (*_data)[1] * vec.y + (*_data)[2];
	auto y = (*_data)[3] * vec.x + (*_data)[4] * vec.y + (*_data)[5];
	auto z = (*_data)[6] * vec.x + (*_data)[7] * vec.y + (*_data)[8];
	return Vector2<T>(x, y);

}

template<class T>
Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& mat)const {

	Matrix3x3<T> result;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			T tempResult = static_cast<T>(0);
			for (int k = 0; k < 3; ++k) {
				tempResult += this->lookAt(i, k) * mat.lookAt(k, j);
			}
			result(i, j) = tempResult;
		}
	}

	return result;

}



template<class T>
Matrix3x3<T>  Matrix3x3<T>::operator*(T num)const {

	Matrix3x3<T> result;
	for (int i = 0; i < 9; ++i) {
		(*result._data)[i] = (*_data)[i] * num;
	}
	return result;
}

template<class T>
Matrix3x3<T>  Matrix3x3<T>::operator/(T num)const {

	Matrix3x3<T> result;
	for (int i = 0; i < 9; ++i) {
		(*result._data)[i] = (*_data)[i] / num;
	}
	return result;
}


template<class B>
Matrix3x3<B> operator*(B num, const Matrix3x3<B>& mat) {
	return mat * num;
}

template<class T>
Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T>& mat) {
	(*_data) = (*mat._data);
	return *this;
}

template<class T>
Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& mat)const {
	Matrix3x3<T> result;
	for (int i = 0; i < 9; ++i) {
		(*result._data)[i] = (*_data)[i] + (*mat._data)[i];
	}
	return result;
}


template<class T>
Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& mat)const {
	Matrix3x3<T> result;
	for (int i = 0; i < 9; ++i) {
		(*result._data)[i] = (*_data)[i] - (*mat._data)[i];
	}
	return result;
}


template<class T>
Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& mat) {
	for (int i = 0; i < 9; ++i) {
		(*_data)[i] += (*mat._data)[i];
	}
	return *this;
}


template<class T>
Matrix3x3<T>& Matrix3x3<T>::operator*=(T num) {
	for (int i = 0; i < 9; ++i) {
		(*_data)[i] *= num;
	}
	return *this;
}

template<class T>
T Matrix3x3<T>::det()const {
	auto a11 = (*_data)[0];
	auto a12 = (*_data)[1];
	auto a13 = (*_data)[2];
	auto a21 = (*_data)[3];
	auto a22 = (*_data)[4];
	auto a23 = (*_data)[5];
	auto a31 = (*_data)[6];
	auto a32 = (*_data)[7];
	auto a33 = (*_data)[8];

	return
		(a11 * a22 * a33 +
			a12 * a23 * a31 +
			a13 * a21 * a32 -
			a13 * a22 * a31 -
			a12 * a21 * a33 -
			a11 * a23 * a32);
}


template<class T>
Matrix3x3<T> Matrix3x3<T>::transToLocalMatrix(
	const Vector2<T>& localY,
	const Vector2<T>& localO) {

	//首先我们构造平移矩阵，把localO平移到原点
	Matrix3x3 trans(Vector3<T>(1, 0, -localO.x),
		Vector3<T>(0, 1, -localO.y),
		Vector3<T>(0, 0, 1));

	auto l = localY.getLength();
	auto sin_beta = -localY.cross(Vector2<T>(0, 1)) / l;
	auto cos_beta = localY.dot(Vector2<T>(0, 1)) / l;

	//然后我们构造旋转矩阵
	Matrix3x3 rotation(Vector3<T>(cos_beta, sin_beta, 0),
		Vector3<T>(-sin_beta, cos_beta, 0),
		Vector3<T>(0, 0, 1));

	//顺序不能乘错了
	return rotation * trans;

}


template<class T>
Matrix3x3<T> Matrix3x3<T>::inverse() {
	auto detInverse = 1 / this->det();
	auto a11 = (*_data)[0];
	auto a12 = (*_data)[1];
	auto a13 = (*_data)[2];
	auto a21 = (*_data)[3];
	auto a22 = (*_data)[4];
	auto a23 = (*_data)[5];
	auto a31 = (*_data)[6];
	auto a32 = (*_data)[7];
	auto a33 = (*_data)[8];

	auto b11 = (a22 * a33 - a32 * a23);
	auto b12 = -(a12 * a33 - a32 * a13);
	auto b13 = (a12 * a23 - a22 * a13);
	auto b21 = -(a21 * a33 - a31 * a23);
	auto b22 = (a11 * a33 - a31 * a13);
	auto b23 = -(a11 * a23 - a21 * a13);
	auto b31 = (a21 * a32 - a31 * a22);
	auto b32 = -(a11 * a32 - a31 * a12);
	auto b33 = (a11 * a22 - a21 * a12);

	Vector3<T> vec1(b11, b12, b13);
	Vector3<T> vec2(b21, b22, b23);
	Vector3<T> vec3(b31, b32, b33);

	Matrix3x3<T> result(vec1, vec2, vec3);
	result *= detInverse;
	return result;
}


template<class T>
T Matrix3x3<T>::trace()const {
	return (*_data)[0] + (*_data)[4] + (*_data)[8];
}


#endif