#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"
#include "DenseMatrix.hpp"
//VectorX为行向量
//VectorY为列向量

//为了实现多态，这里的VectorX和vectorY均使用指针形式

template<class T>
class VectorX : public Array<T> {
public:

	VectorX();
	~VectorX();

	size_t dataSize() const;

	VectorX(const vector<T>& data);

	////转置为列向量
	//VectorY TransToColumnVec();

	//重载向量和向量相乘
	T operator*(const VectorX<T>& vel) const;

	//向量和数乘

	VectorXPtr<T> operator*(const T r) const;
	
	template<class T>
	friend T operator*(const T r, const VectorY& vel);

	//行向量和矩阵相乘，注意左乘和右乘的规则是不一样的
	//但是由于我们规定矩阵大小至少为2 x 2，因此对于行向量我们只有右乘矩阵的法则
	
	//右乘矩阵，得到的结果必为一个行向量
	//我们返回一个matrix的指针
	DenseMatrixPtr<T> operator*(const DenseMatrix<T>& matrix) const;


	

protected:


protected:
	size_t _size;


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
size_t VectorX<T>::dataSize() const{
	return _size;
}


template<class T>
VectorX<T>::VectorX(const vector<T>& data) {

}



template<class T>
T VectorX<T>::operator*(const VectorX<T>& vel) const {

}

template<class T>
VectorXPtr<T> VectorX<T>::operator*(const T r) const {

}

template<class T>
T operator*(const T r, const VectorY<T>& vel) {

}

template<class T>
DenseMatrixPtr<T> VectorX<T>::operator*(const DenseMatrix<T>& matrix) const {

}






template<class T>
class VectorY : public VectorX<T> {
public:
	VectorY();
	~VectorY();

	VectorY(const vector<T>& data);

	////转置为行向量
	//VectorX TransToRowVec();
};

template<class T>
VectorY<T>::VectorY() {

}

template<class T>
VectorY<T>::~VectorY() {

}

template<typename T>
using VectorYPtr = shared_ptr<VectorY<T>>;

#endif
