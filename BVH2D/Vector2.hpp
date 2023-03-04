#pragma once
#include <cmath>
#include <iostream>


template<class T>
class Vector2 {
public:
	Vector2();

	Vector2(const T& _x, const T& _y);

	//拷贝构造函数
	Vector2(const Vector2<T>& var);


	//运算符重载
	Vector2<T> operator+(const Vector2<T>& vec) const;

	Vector2<T> operator-(const Vector2<T>& vec) const;

	Vector2<T> operator*(const T& r) const;

	Vector2<T> operator*(const Vector2<T>& r) const;

	Vector2<T> operator/(const T& r) const;

	template<typename T>
	friend Vector2<T> operator* (const T& r, const Vector2<T>& obj);

	Vector2<T> operator/(const Vector2<T>& r) const;

	void operator=(const Vector2<T>& vec);

	bool operator!=(const Vector2<T>& r)const;

	bool operator==(const Vector2<T>& r)const;

	Vector2<T> operator+=(const Vector2<T>& vec);

	Vector2<T> operator-=(const Vector2<T>& vec);

	Vector2<T> operator*=(const T& r);

	Vector2<T> operator*=(const Vector2<T>& r);

	Vector2<T> operator/=(const T& r);

	Vector2<T> operator/=(const Vector2<T>& r);

	//点积运算
	T dot(const Vector2<T>& vec) const;

	//获取向量长度
	T getLength()const;

	//获取向量长度的平方
	T getLengthSquared()const;

	//向量单位化，改变当前对象的属性
	Vector2<T> normalize();

	//获取单位化后的结果，但并不改变当前对象属性
	Vector2<T> getNormalize()const;

	//叉积运算
	T cross(const Vector2<T>& vec)const;

	//向量外积


	//求两vector之间的距离，此时vector代表点
	T dis(const Vector2<T>& vec)const;

	//求两vector之间的距离的平方，此时vector代表点
	T disSquare(const Vector2<T>& vec)const;

	static Vector2<T> zero() {
		return Vector2<T>(0, 0);
	}



public:
	T x;
	T y;

};

using Vector2D = Vector2<double>;
using Vector2F = Vector2<float>;
using Vector2I = Vector2<int>;



template<class T>
Vector2<T>::Vector2() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0)) {}




template<class T>
Vector2<T>::Vector2(const T& _x, const T& _y) :
	x(_x),
	y(_y) {}


template<class T>
Vector2<T>::Vector2(const Vector2<T>& var) {
	this->x = var.x;
	this->y = var.y;
}

template<class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& vec) const {
	Vector2<T> result(this->x + vec.x, this->y + vec.y);
	return result;
}

template<class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& vec) const {
	Vector2<T> result(this->x - vec.x, this->y - vec.y);
	return result;
}

template<class T>
Vector2<T> Vector2<T>::operator*(const T& r) const {
	Vector2<T> result(this->x * r, this->y * r);
	return result;
}

template<class T>
Vector2<T> Vector2<T>::operator*(const Vector2<T>& r) const {
	Vector2<T> result(this->x * r.x, this->y * r.y);
	return result;
}

template<class T>
Vector2<T> Vector2<T>::operator/(const T& r) const {
	Vector2<T> result(this->x / r, this->y / r);
	return result;
}

template<class T>
Vector2<T> operator* (const T& r, const Vector2<T>& obj) {
	return obj * r;
}

template<class T>
Vector2<T> Vector2<T>::operator/(const Vector2<T>& r) const {
	Vector2<T> result(this->x / r.x, this->y / r.y);
	return result;
}

template<class T>
void Vector2<T>::operator=(const Vector2<T>& vec) {
	this->x = vec.x;
	this->y = vec.y;
}


//浮点数比较大小
template<class T>
bool Vector2<T>::operator!=(const Vector2<T>& r)const {
	if (this->x == r.x && this->y == r.y) {
		return false;
	}
	return true;
}

template<class T>
bool Vector2<T>::operator==(const Vector2<T>& r)const {
	if (this->x == r.x && this->y == r.y) {
		return true;
	}
	return false;
}

template<class T>
Vector2<T> Vector2<T>::operator+=(const Vector2<T>& vec) {
	(*this) = (*this) + vec;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator-=(const Vector2<T>& vec) {
	(*this) = (*this) - vec;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator*=(const T& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator*=(const Vector2<T>& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator/=(const T& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator/=(const Vector2<T>& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
T Vector2<T>::dot(const Vector2<T>& vec) const {
	return this->x * vec.x + this->y * vec.y;
}

template<class T>
T Vector2<T>::getLength()const {
	T length = sqrt(this->x * this->x + this->y * this->y);
	return length;
}


template<class T>
T Vector2<T>::getLengthSquared()const {
	T length = (this->x * this->x + this->y * this->y);
	return length;
}

template<class T>
Vector2<T> Vector2<T>::normalize() {
	double length = getLength();
	if (length > 0) {
		this->x /= length;
		this->y /= length;
	}

	return *this;
}

template<class T>
Vector2<T> Vector2<T>::getNormalize()const {
	T length = getLength();
	if (length > 0) {
		return Vector2<T>(x / length, y / length);
	}

	return *this;
}

template<class T>
T Vector2<T>::cross(const Vector2<T>& vec)const {
	return (x * vec.y - vec.x * y);
}

template<class T>
T Vector2<T>::dis(const Vector2<T>& vec)const {
	auto result = sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
	return result;
}

template<class T>
T Vector2<T>::disSquare(const Vector2<T>& vec)const {
	return dis(vec) * dis(vec);
}



