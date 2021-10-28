#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"



//若构造的类为模板类，那么派生类不可以直接使用继承到的基类数据和方法，
//需要通过this指针使用。否则，在使用一些较新的编译器时，会报“找不到标识符”错误


template<class T>
class VectorN : public Array<T> {
public:

	template<typename T>
	using VectorNPtr = shared_ptr<VectorN<T>>;


	VectorN();
	~VectorN();

	size_t dataSize() const;

	VectorN(const vector<T>& data);


	//重载向量和向量相乘
	T operator*(const VectorN<T>& vel) const;

	//向量和数乘

	VectorNPtr<T> operator*(const T r) const;
	
	template<class T>
	friend VectorNPtr<T> operator*(const T r, const VectorN<T>& vel);

	//向量相加
	VectorNPtr<T> operator+(const VectorN<T>& vel) const;

	//向量相减
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
