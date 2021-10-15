#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"

//VectorXΪ������
//VectorYΪ������


template<class T>
class VectorX : public Array<T> {
public:

	VectorX();
	~VectorX();

	size_t dataSize() const;

	//VectorX(const vector<baseType>& data);

	////ת��Ϊ������
	//VectorY TransToColumnVec();

	//�����������������

protected:
	size_t _size;


};

template<class T>
VectorX<T>::VectorX() {

}

template<class T>
VectorX<T>::~VectorX() {

}

template<class T>
size_t VectorX<T>::dataSize() const{
	return _size;
}

template<class T>
class VectorY : public VectorX<T> {
public:
	VectorY();
	~VectorY();

	//VectorY(const vector<baseType>& data);

	////ת��Ϊ������
	//VectorX TransToRowVec();
};

template<class T>
VectorY<T>::VectorY() {

}

template<class T>
VectorY<T>::~VectorY() {

}



#endif
