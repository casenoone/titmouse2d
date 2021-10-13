/*
**����
* ����titmouse2d�ĵ�һ��ͷ�ļ�
* ��д��2021.10.03
**����
*/
#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>



template<class T>
class Vector2 {
public:
	Vector2();
	
	~Vector2();
	
	Vector2(const T& _x, const T& _y);
	
	//�������캯��
	Vector2(const Vector2<T>& var);

	//��s�����õ�ǰ���������
	inline void set(const T& s);

	//���������
	inline Vector2<T> operator+(const Vector2<T>& vec) const;

	inline Vector2<T> operator-(const Vector2<T>& vec) const;

	inline Vector2<T> operator*(const T& r) const;

	inline Vector2<T> operator*(const Vector2<T>& r) const;

	inline Vector2<T> operator/(const T& r) const;

	inline Vector2<T> operator/(const Vector2<T>& r) const;

	inline void operator=(const Vector2<T>& vec);

	inline bool operator!=(const Vector2<T>& r)const;

	inline bool operator==(const Vector2<T>& r)const;

	inline Vector2<T> operator+=(const Vector2<T>& vec) ;

	inline Vector2<T> operator-=(const Vector2<T>& vec) ;

	inline Vector2<T> operator*=(const T& r) ;

	inline Vector2<T> operator*=(const Vector2<T>& r);

	inline Vector2<T> operator/=(const T& r);

	inline Vector2<T> operator/=(const Vector2<T>& r);

	//�������
	inline T dot(const Vector2<T>& vec) const;

	//��ȡ��������
	inline T getLength()const;

	//��ȡ�������ȵ�ƽ��
	inline T getLengthSquared()const;

	//������λ�����ı䵱ǰ���������
	inline Vector2<T> normalize();

	//��ȡ��λ����Ľ�����������ı䵱ǰ��������
	inline Vector2<T> getNormalize()const;

	//�������
	inline T cross(const Vector2<T>& vec)const;

	//����vector֮��ľ��룬��ʱvector�����
	inline T dis(const Vector2<T>& vec)const;

	//����vector֮��ľ����ƽ������ʱvector�����
	inline T disSquare(const Vector2<T>& vec)const;

	//��vector��С�ķ���
	inline T min()const;

	//����(v,v)/this
	inline Vector2<T> rdiv(const T& v)const;
	
	//���㵱ǰvector��normal�ϵ�ͶӰ
	inline Vector2<T> projected(const Vector2<T>& normal) const;

public:
	T x;
	T y;

};




template<class T>
Vector2<T>::Vector2():
x(static_cast<T>(0)),
y(static_cast<T>(0)){}


template<class T>
Vector2<T>::~Vector2() {

}


template<class T>
Vector2<T>::Vector2(const T& _x, const T& _y) :
	x(_x),
	y(_y){}


template<class T>
Vector2<T>::Vector2(const Vector2<T>& var) {
	this->x = var.x;
	this->y = var.y;
}

template<class T>
inline void Vector2<T>::set(const T& s) {
	this->x = s.x;
	this->y = s.y;
}

template<class T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& vec) const {
	Vector2<T> result(this->x + vec.x, this->y + vec.y);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& vec) const {
	Vector2<T> result(this->x - vec.x, this->y - vec.y);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::operator*(const T& r) const {
	Vector2<T> result(this->x * r, this->y * r);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T>& r) const {
	Vector2 result(this->x * r.x, this->y * r.y);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::operator/(const T& r) const {
	Vector2 result(this->x / r, this->y / r);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T>& r) const {
	Vector2 result(this->x / r.x, this->y / r.y);
	return result;
}

template<class T>
inline void Vector2<T>::operator=(const Vector2<T>& vec) {
	this->x = vec.x;
	this->y = vec.y;
}


//�������Ƚϴ�С
template<class T>
inline bool Vector2<T>::operator!=(const Vector2<T>& r)const {
	if (this->x == r.x && this->y == r.y) {
		return false;
	}
	return true;
}

template<class T>
inline bool Vector2<T>::operator==(const Vector2<T>& r)const {
	if (this->x == r.x && this->y == r.y) {
		return true;
	}
	return false;
}

template<class T>
inline Vector2<T> Vector2<T>::operator+=(const Vector2<T>& vec)  {
	(*this) = (*this) + vec;
	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::operator-=(const Vector2<T>& vec)  {
	(*this) = (*this) - vec;
	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::operator*=(const T& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::operator*=(const Vector2<T>& r) {
	(*this) = (*this) * r;
	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::operator/=(const T& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::operator/=(const Vector2<T>& r) {
	(*this) = (*this) / r;
	return *this;
}

template<class T>
inline T Vector2<T>::dot(const Vector2<T>& vec) const {
	return this->x * vec.x + this->y * vec.y;
}

template<class T>
inline T Vector2<T>::getLength()const {
	T length = sqrt(this->x * this->x + this->y * this->y);
	return length;
}


template<class T>
inline T Vector2<T>::getLengthSquared()const {
	T length = (this->x * this->x + this->y * this->y);
	return length;
}

template<class T>
inline Vector2<T> Vector2<T>::normalize() {
	double length = getLength();
	if (length > 0) {
		this->x /= length;
		this->y /= length;
	}

	return *this;
}

template<class T>
inline Vector2<T> Vector2<T>::getNormalize()const {
	T length = getLength();
	if (length > 0) {
		return Vector2<T>(x / length, y / length);
	}

	return *this;
}

template<class T>
inline T Vector2<T>::cross(const Vector2<T>& vec)const {
	return (x * vec.y - vec.x * y);
}

template<class T>
inline T Vector2<T>::dis(const Vector2<T>& vec)const {
	auto result = sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
	return result;
}

template<class T>
inline T Vector2<T>::disSquare(const Vector2<T>& vec)const {
	return dis(vec) * dis(vec);
}

template<class T>
inline T Vector2<T>::min()const {
	return std::min(x, y);
}

template<class T>
inline Vector2<T> Vector2<T>::rdiv(const T& v)const {
	Vector2 result(v / this->x, v / this->y);
	return result;
}

template<class T>
inline Vector2<T> Vector2<T>::projected(const Vector2<T>& normal) const {
	return *this - (normal * (this->dot(normal)));
}

#endif