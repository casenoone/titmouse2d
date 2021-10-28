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
	using VectorNPtr = shared_ptr<VectorN<T>>;


	VectorN();
	~VectorN();

	size_t dataSize() const;

	VectorN(const vector<T>& data);


	//�����������������
	T operator*(const VectorN<T>& vel) const;

	//����������

	VectorNPtr<T> operator*(const T r) const;
	
	template<class T>
	friend VectorNPtr<T> operator*(const T r, const VectorN<T>& vel);

	//�������
	VectorNPtr<T> operator+(const VectorN<T>& vel) const;

	//�������
	VectorNPtr<T> operator-(const VectorN<T>& vel) const;

};


template<typename T>
using VectorNPtr = shared_ptr<VectorN<T>>;


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
size_t VectorN<T>::dataSize() const {
	return this->_size;
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
VectorNPtr<T> VectorN<T>::operator*(const T r) const {
	vector<T> temp;
	temp.resize(this->_size);
	this->forEachIndex([&](size_t i) {
		temp[i] = this->lookAt(i) * r;
	});

	VectorNPtr<T> result = make_shared<VectorN<T>>(temp);
	return result;

}



template<class T>
VectorNPtr<T> operator*(const T r, const VectorN<T>& vel) {
	return (vel * r);
}


template<class T>
VectorNPtr<T> VectorN<T>::operator+(const VectorN<T>& vel) const {
	
	vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](size_t i) {
		temp[i] = vel.lookAt(i) + this->lookAt(i);
		
	});

	auto resultVec = make_shared<VectorN>(temp);
	return resultVec;
}

template<class T>
VectorNPtr<T> VectorN<T>::operator-(const VectorN<T>& vel) const {
	vector<T> temp;
	temp.resize(vel.dataSize());

	vel.forEachIndex([&](size_t i) {
		temp[i] = this->lookAt(i)-vel->lookAt(i);

		});

	auto resultVec = make_shared<VectorN>(temp);
	return resultVec;
}






#endif
