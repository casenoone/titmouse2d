#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>


template<class T>
class Vector3 {
public:
	Vector3();

	~Vector3();

	Vector3(const T& _x, const T& _y, const T& _z);

	//拷贝构造函数
	Vector3(const Vector3<T>& var);

	//用s来设置当前对象的属性
	void set(const T& s);

	//运算符重载
	Vector3<T> operator+(const Vector3<T>& vec) const;

	Vector3<T> operator-(const Vector3<T>& vec) const;

	Vector3<T> operator*(const T& r) const;

	Vector3<T> operator*(const Vector3<T>& r) const;

	Vector3<T> operator/(const T& r) const;

	template<typename T>
	friend Vector3<T> operator* (const T& r, const Vector3<T>& obj);

	Vector3<T> operator/(const Vector3<T>& r) const;

	bool operator!=(const Vector3<T>& r)const;

	bool operator==(const Vector3<T>& r)const;

	Vector3<T> operator+=(const Vector3<T>& vec);

	Vector3<T> operator-=(const Vector3<T>& vec);

	Vector3<T> operator*=(const T& r);

	Vector3<T> operator*=(const Vector3<T>& r);

	Vector3<T> operator/=(const T& r);

	Vector3<T> operator/=(const Vector3<T>& r);

	//点积运算
	T dot(const Vector3<T>& vec) const;

	//获取向量长度
	T getLength()const;

	//获取向量长度的平方
	T getLengthSquared()const;

	//向量单位化，改变当前对象的属性
	Vector3<T> normalize();

	//获取单位化后的结果，但并不改变当前对象属性
	Vector3<T> getNormalize()const;

	//叉积运算
	Vector3<T> cross(const Vector3<T>& vec)const;

	//求两vector之间的距离，此时vector代表点
	T dis(const Vector3<T>& vec)const;

	//求两vector之间的距离的平方，此时vector代表点
	T disSquare(const Vector3<T>& vec)const;

	//求vector最小的分量
	T min()const;

	//返回(v,v)/this
	Vector3<T> rdiv(const T& v)const;

	//计算当前vector在normal上的投影
	Vector3<T> projected(const Vector3<T>& normal) const;

public:
	T x;
	T y;
	T z;
};

using Vector3D = Vector3<double>;
using Vector3F = Vector3<float>;
using Vector3I = Vector3<int>;



template<class T>
Vector3<T>::Vector3() :
	x(static_cast<T>(0)),
	y(static_cast<T>(0)),
	y(static_cast<T>(0)) {}


template<class T>
Vector3<T>::~Vector3() {

}


template<class T>
Vector3<T>::Vector3(const T& _x, const T& _y, const T& _z) :
	x(_x),
	y(_y),
	z(_z) {}


template<class T>
Vector3<T>::Vector3(const Vector3<T>& var) {
	this->x = var.x;
	this->y = var.y;
	this->z = var.z;
}

template<class T>
void Vector3<T>::set(const T& s) {
	this->x = s.x;
	this->y = s.y;
	this->z = s.z;
}

template<class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& vec) const {
	Vector3<T> result(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	return result;
}

template<class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& vec) const {
	Vector3<T> result(this->x - vec.x, this->y - vec.y, this->z - vec.z);
	return result;
}

template<class T>
Vector3<T> Vector3<T>::operator*(const T& r) const {
	Vector3<T> result(this->x * r, this->y * r, this->z * r);
	return result;
}

template<class T>
Vector3<T> Vector3<T>::operator*(const Vector3<T>& r) const {
	Vector3<T> result(this->x * r.x, this->y * r.y, this->z * r.z);
	return result;
}

template<class T>
Vector3<T> Vector3<T>::operator/(const T& r) const {
	Vector3<T> result(this->x / r, this->y / r, this->z / r);
	return result;
}

template<class T>
Vector3<T> operator* (const T& r, const Vector3<T>& obj) {
	return obj * r;
}

template<class T>
Vector3<T> Vector3<T>::operator/(const Vector3<T>& r) const {
	Vector3<T> result(this->x / r.x, this->y / r.y, this->z / r.z);
	return result;
}


//浮点数比较大小
template<class T>
bool Vector3<T>::operator!=(const Vector3<T>& r)const {
	if (this->x == r.x && this->y == r.y && this->z == r.z) {
		return false;
	}
	return true;
}

template<class T>
bool Vector3<T>::operator==(const Vector3<T>& r)const {
	if (this->x == r.x && this->y == r.y && this->z == r.z) {
		return true;
	}
	return false;
}

template<class T>
Vector3<T> Vector3<T>::operator+=(const Vector3<T>& vec) {
	(*this) = (*this) + vec;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator-=(const Vector3<T>& vec) {
	(*this) = (*this) - vec;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator*=(const T& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator*=(const Vector3<T>& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator/=(const T& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator/=(const Vector3<T>& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
T Vector3<T>::dot(const Vector3<T>& vec) const {
	return this->x * vec.x + this->y * vec.y + this->z * vec.z;
}

template<class T>
T Vector3<T>::getLength()const {
	T length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	return length;
}


template<class T>
T Vector3<T>::getLengthSquared()const {
	T length = (this->x * this->x + this->y * this->y + this->z * this->z);
	return length;
}

template<class T>
Vector3<T> Vector3<T>::normalize() {
	double length = getLength();
	if (length > 0) {
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}

	return *this;
}

template<class T>
Vector3<T> Vector3<T>::getNormalize()const {
	T length = getLength();
	if (length > 0) {
		return Vector3<T>(x / length, y / length, z / length);
	}

	return *this;
}

template<class T>
Vector3<T> Vector3<T>::cross(const Vector3<T>& vec)const {
	return Vector3<T>(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

template<class T>
T Vector3<T>::dis(const Vector3<T>& vec)const {
	auto x1 = (vec.x - x) * (vec.x - x);
	auto y1 = (vec.y - y) * (vec.y - y);
	auto z1 = (vec.z - z) * (vec.z - z);
	auto result = sqrt(x1 + y1 + z1);
	return result;
}

template<class T>
T Vector3<T>::disSquare(const Vector3<T>& vec)const {
	return dis(vec) * dis(vec);
}


template<class T>
T Vector3<T>::min()const {
	return std::min(x, y, z);
}

template<class T>
Vector3<T> Vector3<T>::rdiv(const T& v)const {
	Vector3 result(v / this->x, v / this->y, v / this->z);
	return result;
}

template<class T>
Vector3<T> Vector3<T>::projected(const Vector3<T>& normal) const {
	return *this - (normal * (this->dot(normal)));
}

#endif