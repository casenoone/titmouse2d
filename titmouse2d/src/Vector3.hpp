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

	//�������캯��
	Vector3(const Vector3<T>& var);

	//��s�����õ�ǰ���������
	void set(const T& s);

	//���������
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

	//�������
	T dot(const Vector3<T>& vec) const;

	//��ȡ��������
	T getLength()const;

	//��ȡ�������ȵ�ƽ��
	T getLengthSquared()const;

	//������λ�����ı䵱ǰ���������
	Vector3<T> normalize();

	//��ȡ��λ����Ľ�����������ı䵱ǰ��������
	Vector3<T> getNormalize()const;

	//�������
	Vector3<T> cross(const Vector3<T>& vec)const;

	//����vector֮��ľ��룬��ʱvector�����
	T dis(const Vector3<T>& vec)const;

	//����vector֮��ľ����ƽ������ʱvector�����
	T disSquare(const Vector3<T>& vec)const;

	//��vector��С�ķ���
	T min()const;

	//����(v,v)/this
	Vector3<T> rdiv(const T& v)const;

	//���㵱ǰvector��normal�ϵ�ͶӰ
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


//�������Ƚϴ�С
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