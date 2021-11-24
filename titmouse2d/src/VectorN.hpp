#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"


//���������Ϊģ���࣬��ô�����಻����ֱ��ʹ�ü̳е��Ļ������ݺͷ�����
//��Ҫͨ��thisָ��ʹ�á�������ʹ��һЩ���µı�����ʱ���ᱨ���Ҳ�����ʶ��������


template<class T>
class VectorN : public Array<T> {
public:

	template<typename T>
	using vectorNPtr = shared_ptr<VectorN<T>>;


	VectorN();
	~VectorN();

	VectorN(const vector<T>& data);
	
	VectorN(int len);

	T norm() const;

	T norm2() const;

	//�����������������
	T operator*(const VectorN<T>& vel) const;

	//�˵���
	void operator*=(const T& r);

	//����������
	vectorNPtr<T> operator*(const T r) const;
	
	template<class T>
	friend vectorNPtr<T> operator*(const T r, const VectorN<T>& vel);

	//�������
	vectorNPtr<T> operator+(const VectorN<T>& vel) const;

	//�������
	vectorNPtr<T> operator-(const VectorN<T>& vel) const;

};


template<typename T>
using vectorNPtr = shared_ptr<VectorN<T>>;


template<class T>                                                                            
VectorN<T>::VectorN() {

}

template<class T>
VectorN<T>::~VectorN() {

}


template<class T>
VectorN<T>::VectorN(const vector<T>& data): Array<T>(data) {

}

template<class T>
VectorN<T>::VectorN(int len) {
	this->reSize(len, static_cast<T>(0));
}



template<class T>
T VectorN<T>::operator*(const VectorN<T>& vel) const {
	T result = static_cast<T>(0);
	
	vel.forEachIndex([&](size_t i){
		result += this->lookAt(i) * (vel.lookAt(i));
	});
	return result;
}


template<class T>
void VectorN<T>::operator*=(const T& r) {
	this->forEachIndex([&](size_t i) {
		(*this)[i] *= r;
		});
}


template<class T>
vectorNPtr<T> VectorN<T>::operator*(const T r) const {
	vector<T> temp;
	temp.resize(this->_size);
	this->forEachIndex([&](size_t i) {
		temp[i] = this->lookAt(i) * r;
	});


	vectorNPtr<T> result = make_shared<VectorN<T>>(temp);
	return result;

}



template<class T>
vectorNPtr<T> operator*(const T r, const VectorN<T>& vel) {
	return (vel * r);
}


template<class T>
vectorNPtr<T> VectorN<T>::operator+(const VectorN<T>& vel) const {
	
	vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](size_t i) {
		temp[i] = vel.lookAt(i) + this->lookAt(i);
		
	});

	vectorNPtr<T> resultVec = make_shared<VectorN>(temp);
	return resultVec;
}

template<class T>
vectorNPtr<T> VectorN<T>::operator-(const VectorN<T>& vel) const {
	vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](size_t i) {
		temp[i] = this->lookAt(i)-vel.lookAt(i);

		});

	vectorNPtr<T> resultVec = make_shared<VectorN>(temp);
	return resultVec;
}

template<class T>
T VectorN<T>::norm() const{
	return sqrt(this->norm2());
}



template<class T>
T VectorN<T>::norm2() const{
	T result = static_cast<T>(0);
	this->forEachIndex([&](size_t i) {
		result += this->lookAt(i) * this->lookAt(i);
	});
	return result;
}


#endif
