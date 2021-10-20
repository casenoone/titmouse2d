#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Array.hpp"
#include "DenseMatrix.hpp"
//VectorXΪ������
//VectorYΪ������

//Ϊ��ʵ�ֶ�̬�������VectorX��vectorY��ʹ��ָ����ʽ

template<class T>
class VectorX : public Array<T> {
public:

	VectorX();
	~VectorX();

	size_t dataSize() const;

	VectorX(const vector<T>& data);

	////ת��Ϊ������
	//VectorY TransToColumnVec();

	//�����������������
	T operator*(const VectorX<T>& vel) const;

	//����������

	VectorXPtr<T> operator*(const T r) const;
	
	template<class T>
	friend T operator*(const T r, const VectorY& vel);

	//�������;�����ˣ�ע����˺��ҳ˵Ĺ����ǲ�һ����
	//�����������ǹ涨�����С����Ϊ2 x 2����˶�������������ֻ���ҳ˾���ķ���
	
	//�ҳ˾��󣬵õ��Ľ����Ϊһ��������
	//���Ƿ���һ��matrix��ָ��
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

	////ת��Ϊ������
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
