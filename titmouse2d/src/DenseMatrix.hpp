#ifndef DENSEMATRIX_HPP
#define DENSEMATRIX_HPP

#include <iostream>
using namespace std;

#include <vector>

#include "VectorN.hpp"
#include "Size2.h"
#include "ConstVar.h"

//Ӧ��дһ��Matrix����
//ϡ�����+���ܾ���

//������������������

//���ǹ涨��һ������Ĵ�С������2 x 2��1 x n��n x 1ʹ��VectorN

//���ھ������е�Ҫ�󣬽���ʹ����ȥ�ж�
//��������������ˣ�Ҫ����ָ����������



//���������Ⱥ�������
const int ROW = 0;
const int COLUMN = 1;

template<class T>
class DenseMatrix {
public:

	template<typename T>
	using DenseMatrixPtr = shared_ptr<DenseMatrix<T>>;


	DenseMatrix();
	~DenseMatrix();

	DenseMatrix(int row, int column, const vector<vector<T>>& data);

	DenseMatrix(const DenseMatrix<T>& other);

	Size2 dataSize() const;

	void reSize(Size2 newSize);

	template<typename Callback>
	void forEachIndex(Callback func) const;

	//���������ȡ�������
	void setOrder(int state);

	//���ؾ����������Ȼ���������
	int getOrder() const;

	T& operator()(int i, int j);

	T& lookAt(int i, int j) const;

	//��������������������
	vectorNPtr<T> getVec(int idx) const;

	//�����������
	DenseMatrixPtr<T> operator+(const DenseMatrix<T>& mat) const;

	//�����������
	DenseMatrixPtr<T> operator-(const DenseMatrix<T>& mat) const;

	//�����������
	DenseMatrixPtr<T> operator*(const DenseMatrix<T>& mat)const;

	//����������
	DenseMatrixPtr<T> operator*(const T r) const;

	//���������˵���Ԫʵ��
	template<class T>
	friend DenseMatrixPtr<T> operator*(const T r, const DenseMatrix<T>& mat);

	//����������
	DenseMatrixPtr<T> operator/(const T r) const;

private:
	Size2 _size;

	vector<vectorNPtr<T>> _data;

	//Ĭ��Ϊ�����ȴ洢
	int _order = ROW;

};

template<typename T>
using DenseMatrixPtr = shared_ptr<DenseMatrix<T>>;

template<class T>
DenseMatrix<T>::DenseMatrix() {

}

template<class T>
DenseMatrix<T>::~DenseMatrix() {

}

template<class T>
DenseMatrix<T>::DenseMatrix(int row, int column, const vector<vector<T>>& data) {

	_size = Size2(row, column);

	for (int j = 0; j < column; ++j) {

		vector<T> temp;

		for (int i = 0; i < row; ++i) {
			temp.push_back(data[i][j]);
		}

		vectorNPtr<T> temp_p = make_shared<VectorN<T>>(temp);
		_data.push_back(temp_p);

	}

}


//����Ҫ���һ����������ĳߴ��Ƿ���ͬ
//��ҪһЩ���ϼ��Ķ�����Ŀǰ�Ȳ���
template<class T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix<T>& other) {
	_data = other._data;
}

template<class T>
Size2 DenseMatrix<T>::dataSize() const {
	return _size;
}

//����һ�£����ǲ��ṩ���������
//����һ��������˵��һ����ȷ�����Ͳ��������޸���ߴ���
//��Ȼ���޸ĳߴ�������Ǻβ�������һ������
template<class T>
void DenseMatrix<T>::reSize(Size2 newSize) {

}

template<typename T>
template<typename Callback>
void DenseMatrix<T>::forEachIndex(Callback func) const {
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.y; ++j) {
			func(i, j);
		}
	}
}


//���������ȡ�������
template<typename T>
void DenseMatrix<T>::setOrder(int state) {
	_order = state;
}

//���ؾ����������Ȼ���������
template<typename T>
int DenseMatrix<T>::getOrder() const {
	return _order;
}

template<typename T>
vectorNPtr<T> DenseMatrix<T>::getVec(int idx) const {

	return _data[idx];
}


template<typename T>
T& DenseMatrix<T>::lookAt(int i, int j) const {
	auto result = 0;

	//�����������
	if (_order == ROW) {
		return (*_data[j])[i];
	}
	//�����������
	else {
		return (*_data[i])[j];
	}
}

template<typename T>
T& DenseMatrix<T>::operator()(int i, int j) {
	auto result = 0;

	//�����������
	if (_order == COLUMN) {
		return (*_data[j])[i];
	}
	//�����������
	else {
		return (*_data[i])[j];
	}
}


//�����������
//����ҲҪע���һ���쳣��⣬����������ߴ粻ͬʱ�޷����
template<typename T>
DenseMatrixPtr<T> DenseMatrix<T>::operator+(const DenseMatrix<T>& mat) const {
	if (mat._size == this->_size) {

		vector<vector<T>> temp(_size.x, vector<T>(_size.y));

		this->forEachIndex([&](int i, int j) {

			temp[i][j] = this->lookAt(i, j) + mat.lookAt(i, j);

			});

		DenseMatrixPtr<T> result = make_shared<DenseMatrix<T>>(_size.x, _size.y, temp);
		return result;

	}
}



//�����������
template<typename T>
DenseMatrixPtr<T> DenseMatrix<T>::operator-(const DenseMatrix<T>& mat) const {
	if (mat._size == this->_size) {

		vector<vector<T>> temp(_size.x, vector<T>(_size.y));

		this->forEachIndex([&](int i, int j) {

			temp[i][j] = this->lookAt(i, j) - mat.lookAt(i, j);

			});

		DenseMatrixPtr<T> result = make_shared<DenseMatrix<T>>(_size.x, _size.y, temp);
		return result;

	}
}



//�����������
template<class T>
DenseMatrixPtr<T> DenseMatrix<T>::operator*(const DenseMatrix<T>& mat)const {
	if (this->_size.y == mat._size.x) {
		auto newSize = Size2(this->_size.x, mat._size.y);
		vector<vector<T>> temp(newSize.x, vector<T>(newSize.y));

		for (int i = 0; i < newSize.x; ++i) {
			for (int j = 0; j < newSize.y; ++j) {
				T tempResult = static_cast<T>(0);
				for (int k = 0; k < this->_size.y; ++k) {
					tempResult += this->lookAt(i, k) * mat.lookAt(k, j);
				}
				temp[i][j] = tempResult;
			}
		}

		DenseMatrixPtr<T> result = make_shared<DenseMatrix>(newSize.x, newSize.y, temp);
		return result;

	}
}

//����������
template<class T>
DenseMatrixPtr<T> DenseMatrix<T>::operator*(const T r) const {
	vector<vector<T>> temp(_size.x, vector<T>(_size.y));
	this->forEachIndex([&](int i, int j) {
		temp[i][j] = r * this->lookAt(i, j);
		});
	DenseMatrixPtr<T> result = make_shared<DenseMatrix>(_size.x, _size.y, temp);
	return result;
}


template<class T>
DenseMatrixPtr<T> operator*(const T r, const DenseMatrix<T>& mat) {
	return mat * r;
}



template<class T>
DenseMatrixPtr<T> DenseMatrix<T>::operator/(const T r) const {
	vector<vector<T>> temp(_size.x, vector<T>(_size.y));
	this->forEachIndex([&](int i, int j) {
		temp[i][j] = r / this->lookAt(i, j);
		});
	DenseMatrixPtr<T> result = make_shared<DenseMatrix>(_size.x, _size.y, temp);
	return result;
}




#endif