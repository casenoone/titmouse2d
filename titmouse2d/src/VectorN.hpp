#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"

//VectorXΪ������
//VectorYΪ������

//Ϊ��ʵ�ֶ�̬�������VectorX��vectorY��ʹ��ָ����ʽ

//��θ�������ʼ���أ����ﻹû�����ôд���Ƿ���Ҫbuilder�໹û��ã���ʱ�ͼ򵥴ֱ��ĵ��ù��캯����


//���������Ϊģ���࣬��ô�����಻����ֱ��ʹ�ü̳е��Ļ������ݺͷ�����
//��Ҫͨ��thisָ��ʹ�á�������ʹ��һЩ���µı�����ʱ���ᱨ���Ҳ�����ʶ��������


template<class T>
class VectorX : public Array<T> {
public:

	template<typename T>
	using VectorXPtr = shared_ptr<VectorX<T>>;


	VectorX();
	~VectorX();

	size_t dataSize() const;

	VectorX(const vector<T>& data);

	//ת��Ϊ������
	//VectorY TransToColumnVec();

	//�����������������
	T operator*(const VectorX<T>& vel) const;

	//����������

	VectorXPtr<T> operator*(const T r) const;
	
	template<class T>
	friend VectorXPtr<T> operator*(const T r, const VectorX<T>& vel);

};


template<typename T>
using VectorXPtr = shared_ptr<VectorX<T>>;


template<class T>                                                                            
VectorX<T>::VectorX() {

}

template<class T>
VectorX<T>::~VectorX() {

}


template<class T>
VectorX<T>::VectorX(const vector<T>& data): Array<T>(data) {

}


template<class T>
size_t VectorX<T>::dataSize() const {
	return this->_size;
}


template<class T>
T VectorX<T>::operator*(const VectorX<T>& vel) const {
	T result = static_cast<T>(0);
	
	vel.forEachIndex([&](size_t i){
		result += this->lookAt(i) * (vel.lookAt(i));
	});
	return result;
}




template<class T>
VectorXPtr<T> VectorX<T>::operator*(const T r) const {
	vector<T> temp;
	temp.resize(this->_size);
	this->forEachIndex([&](size_t i) {
		temp[i] = this->lookAt(i) * r;
	});

	VectorXPtr<T> result = make_shared<VectorX<T>>(temp);
	return result;

}



template<class T>
VectorXPtr<T> operator*(const T r, const VectorX<T>& vel) {
	return (vel * r);
}





template<class T>
class VectorY : public VectorX<T> {
public:

	template<typename T>
	using VectorYPtr = shared_ptr<VectorY<T>>;


	VectorY();
	~VectorY();

	size_t dataSize() const;

	VectorY(const vector<T>& data);

	//VectorY TransToColumnVec();

	//�����������������
	T operator*(const VectorX<T>& vel) const;

	//����������

	VectorYPtr<T> operator*(const T r) const;

	template<class T>
	friend VectorYPtr<T> operator*(const T r, const VectorY<T>& vel);


};


template<typename T>
using VectorYPtr = shared_ptr<VectorY<T>>;


template<class T>
VectorY<T>::VectorY() {

}

template<class T>
VectorY<T>::~VectorY() {

}


template<class T>
VectorY<T>::VectorY(const vector<T>& data):VectorX<T>(data){
	
}


template<class T>
size_t VectorY<T>::dataSize() const {
	return this->_size;
}


template<class T>
T VectorY<T>::operator*(const VectorX<T>& vel) const {
	T result = static_cast<T>(0);

	vel.forEachIndex([&](size_t i) {
		result += this->lookAt(i) * vel[i];
		});
	return result;
}




template<class T>
VectorYPtr<T> VectorY<T>::operator*(const T r) const {
	vector<T> temp;
	temp.resize(this->_size);
	this->forEachIndex([&](size_t i) {
		temp[i] = this.lookAt(i) * r;
		});

	VectorYPtr<T> result = make_shared<VectorY<T>>(temp);
	return result;

}



template<class T>
VectorYPtr<T> operator*(const T r, const VectorY<T>& vel) {
	return (vel * r);
}





#endif
