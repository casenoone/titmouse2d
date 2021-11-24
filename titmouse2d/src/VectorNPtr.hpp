#ifndef VectorNPTR_HPP
#define VectorNPTR_HPP

#include "VectorN.hpp"
#include "ArrayPtr.hpp"

template<class T>
class VectorNPtr : public ArrayPtr<T>{
public:

	VectorNPtr();

	virtual ~VectorNPtr();

	VectorNPtr(const vector<T>& data);

	VectorNPtr(int len);

	//这个构造函数一般用于重载运算符处
	VectorNPtr(const shared_ptr<VectorN<T>>& p);

	T norm() const;

	T norm2() const;
	
	

	//重载向量和向量相乘
	T operator*(const VectorNPtr<T>& vel) const;

	void operator*=(const T & r);

	//向量和数乘

	VectorNPtr<T> operator*(const T r) const;

	template<class T>
	friend vectorNPtr<T> operator*(const T r, const VectorNPtr<T>& vel);

	//向量相加
	VectorNPtr<T> operator+(const VectorNPtr<T>& vel) const;

	//向量相减
	VectorNPtr<T> operator-(const VectorNPtr<T>& vel) const;

public:
	shared_ptr<VectorN<T>> getVectorNPtr()const;

private:
	shared_ptr<VectorN<T>> vectorNPtr;
};


template<class T>
VectorNPtr<T>::VectorNPtr() {
	vector<T> temp;
	vectorNPtr = make_shared<VectorN<T>>(temp);
	this->arrayPtr = this->vectorNPtr;
}

template<class T>
VectorNPtr<T>::VectorNPtr(const shared_ptr<VectorN<T>>& p) {
	this->vectorNPtr = p;
	this->arrayPtr = this->vectorNPtr;
}

template<class T>
VectorNPtr<T>::~VectorNPtr() {

}

//一失足成千古恨，这里本没必要搞这个类的
//但是。。。等写3D版本的时候再改正吧
template<class T>
VectorNPtr<T>::VectorNPtr(const vector<T>& data){
	vectorNPtr = make_shared<VectorN<T>>(data);	
	//绑定基类指针
	this->arrayPtr = this->vectorNPtr;
	
}

template<class T>
VectorNPtr<T>::VectorNPtr(int len) {
	vectorNPtr = make_shared<VectorN<T>>(len);
	this->arrayPtr = this->vectorNPtr;
}


template<class T>
shared_ptr<VectorN<T>> VectorNPtr<T>::getVectorNPtr() const{
	
	return vectorNPtr;
}


template<class T>
T VectorNPtr<T>::operator*(const VectorNPtr<T>& vel) const {
	
	T k = (*(this->vectorNPtr)) * (*(vel.vectorNPtr));
	return k;
}

template<class T>
void VectorNPtr<T>::operator*=(const T& r) {
	(*this->vectorNPtr) *= r;
}


template<class T>
VectorNPtr<T> VectorNPtr<T>::operator*(const T r) const {
	VectorNPtr<T> result((*(this->vectorNPtr)) * r);
	return result;
}



template<class T>
vectorNPtr<T> operator*(const T r, const VectorNPtr<T>& vel) {
	return (*(vel.vectorNPtr) * r);
}


template<class T>
VectorNPtr<T> VectorNPtr<T>::operator+(const VectorNPtr<T>& vel) const {

	VectorNPtr<T> result(*(this->vectorNPtr) + (*(vel.vectorNPtr)));

	return result;
}

template<class T>
VectorNPtr<T> VectorNPtr<T>::operator-(const VectorNPtr<T>& vel) const {
	
	VectorNPtr<T> result;
	result.vectorNPtr = *(this->vectorNPtr) - (*(vel.vectorNPtr));
	return result;
}

template<class T>
T VectorNPtr<T>::norm() const {
	return this->vectorNPtr->norm();
}



template<class T>
T VectorNPtr<T>::norm2() const {
	return this->vectorNPtr->norm2();
}


#endif