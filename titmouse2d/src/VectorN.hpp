#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
#include <vector>

#include "Array.hpp"


//���������Ϊģ���࣬��ô�����಻����ֱ��ʹ�ü̳е��Ļ������ݺͷ�����
//��Ҫͨ��thisָ��ʹ�á�������ʹ��һЩ���µı�����ʱ���ᱨ���Ҳ�����ʶ��������


template<class T>
class VectorN : public Array<T> {
public:

	VectorN();

	VectorN(const std::vector<T>& data);

	VectorN(int len);

	T norm() const;

	T norm2() const;

	//�����������������
	T operator*(const VectorN<T>& vel) const;

	//�˵���
	void operator*=(const T& r);

	//����������
	VectorN<T> operator*(const T r) const;

	template<class T>
	friend VectorN<T> operator*(const T r, const VectorN<T>& vel);

	//�������
	VectorN<T> operator+(const VectorN<T>& vel) const;

	//�������
	VectorN<T> operator-(const VectorN<T>& vel) const;

};


template<class T>
VectorN<T>::VectorN() :Array<T>() {

}


template<class T>
VectorN<T>::VectorN(const std::vector<T>& data) : Array<T>(data) {

}

template<class T>
VectorN<T>::VectorN(int len) : Array<T>(len) {

}



template<class T>
T VectorN<T>::operator*(const VectorN<T>& vel) const {
	T result = static_cast<T>(0);

	vel.forEachIndex([&](int i) {
		result += this->lookAt(i) * (vel.lookAt(i));
		});
	return result;
}


template<class T>
void VectorN<T>::operator*=(const T& r) {
	this->forEachIndex([&](int i) {
		(*this)[i] *= r;
		});
}


template<class T>
VectorN<T> VectorN<T>::operator*(const T r) const {
	std::vector<T> temp;
	temp.resize(this->_size);
	this->forEachIndex([&](int i) {
		temp[i] = this->lookAt(i) * r;
		});


	VectorN<T> result(temp);
	return result;

}



template<class T>
VectorN<T> operator*(const T r, const VectorN<T>& vel) {
	return (vel * r);
}


template<class T>
VectorN<T> VectorN<T>::operator+(const VectorN<T>& vel) const {

	std::vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](int i) {
		temp[i] = vel.lookAt(i) + this->lookAt(i);

		});

	VectorN<T> resultVec(temp);
	return resultVec;
}

template<class T>
VectorN<T> VectorN<T>::operator-(const VectorN<T>& vel) const {
	std::vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](int i) {
		temp[i] = this->lookAt(i) - vel.lookAt(i);

		});

	VectorN<T> resultVec(temp);
	return resultVec;
}

template<class T>
T VectorN<T>::norm() const {
	return sqrt(this->norm2());
}



template<class T>
T VectorN<T>::norm2() const {
	T result = static_cast<T>(0);
	this->forEachIndex([&](int i) {
		result += this->lookAt(i) * this->lookAt(i);
		});
	return result;
}


#endif
