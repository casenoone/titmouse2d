#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"

//VectorX为行向量
//VectorY为列向量


template<class T>
class VectorX : public Array<T> {
public:

	VectorX();
	~VectorX();

	size_t dataSize() const;

	//VectorX(const vector<baseType>& data);

	////转置为列向量
	//VectorY TransToColumnVec();

	//重载向量和向量相乘

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

	////转置为行向量
	//VectorX TransToRowVec();
};

template<class T>
VectorY<T>::VectorY() {

}

template<class T>
VectorY<T>::~VectorY() {

}



#endif
