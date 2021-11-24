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

	//������캯��һ�����������������
	VectorNPtr(const shared_ptr<VectorN<T>>& p);

	T norm() const;

	T norm2() const;
	
	

	//�����������������
	T operator*(const VectorNPtr<T>& vel) const;

	void operator*=(const T & r);

	//����������

	VectorNPtr<T> operator*(const T r) const;

	template<class T>
	friend vectorNPtr<T> operator*(const T r, const VectorNPtr<T>& vel);

	//�������
	VectorNPtr<T> operator+(const VectorNPtr<T>& vel) const;

	//�������
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

//һʧ���ǧ�źޣ����ﱾû��Ҫ��������
//���ǡ�������д3D�汾��ʱ���ٸ�����
template<class T>
VectorNPtr<T>::VectorNPtr(const vector<T>& data){
	vectorNPtr = make_shared<VectorN<T>>(data);	
	//�󶨻���ָ��
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