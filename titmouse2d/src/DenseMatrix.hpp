#ifndef DENSEMATRIX_HPP
#define DENSEMATRIX_HPP

#include <iostream>

#include <vector>

#include "VectorN.hpp"
#include "Size2.h"
#include "ConstVar.h"

//默认行优先

template<class T>
class DenseMatrix {
public:

	DenseMatrix() = default;

	DenseMatrix(int row, int column);

	DenseMatrix(int row, int column, const std::vector<std::vector<double>>& data);

	DenseMatrix(Size2 order);

	Size2 dataSize() const;

	template<typename Callback>
	void forEachIndex(Callback func) const;


	T& operator()(int i, int j);

	T lookAt(int i, int j) const;

	DenseMatrix<T> operator=(const DenseMatrix<T>& mat);

	//两个矩阵相加
	DenseMatrix<T> operator+(const DenseMatrix<T>& mat) const;

	//两个矩阵相减
	DenseMatrix<T> operator-(const DenseMatrix<T>& mat) const;

	//两个矩阵相乘
	DenseMatrix<T> operator*(const DenseMatrix<T>& mat)const;

	//矩阵和数相乘
	DenseMatrix<T> operator*(const T r) const;

	//矩阵和数相乘的友元实现
	template<class T>
	friend DenseMatrix<T> operator*(const T r, const DenseMatrix<T>& mat);

	//矩阵和数相除
	DenseMatrix<T> operator/(const T r) const;

	//矩阵乘以向量
	VectorN<T> operator*(const VectorN<T>& vec)const;

	//LU分解
	void LU(DenseMatrix<T>& L, DenseMatrix<T>& U)const;


	static DenseMatrix<T> identityMatrix(int row, int column);

private:
	Size2 _size;

	std::shared_ptr<std::vector<T>> _data;

};



template<class T>
DenseMatrix<T>::DenseMatrix(Size2 order) {
	_data = std::make_shared<std::vector<T>>();
	_size = order;
	_data->resize(_size.x * _size.y);
}


template<class T>
DenseMatrix<T>::DenseMatrix(int row, int column) {
	_data = std::make_shared<std::vector<T>>();
	_size = Size2(row, column);
	_data->resize(row * column);
}

template<class T>
DenseMatrix<T>::DenseMatrix(int row, int column, const std::vector<std::vector<double>>& data) {
	_data = std::make_shared<std::vector<T>>();
	_size = Size2(row, column);
	_data->resize(row * column);

	int count = 0;

	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			(*_data)[count++] = data[i][j];
		}
	}
}

template<class T>
Size2 DenseMatrix<T>::dataSize() const {
	return _size;
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

//实现深拷贝
template<typename T>
DenseMatrix<T> DenseMatrix<T>::operator=(const DenseMatrix<T>& mat) {
	(*_data) = *(mat._data);
	_size = mat._size;
}

template<typename T>
T DenseMatrix<T>::lookAt(int i, int j) const {
	return (*_data)[_size.y * i + j];
}

template<typename T>
T& DenseMatrix<T>::operator()(int i, int j) {
	return (*_data)[_size.y * i + j];
}


//两个矩阵相加
//这里也要注意加一个异常检测，当两个矩阵尺寸不同时无法相加
template<typename T>
DenseMatrix<T> DenseMatrix<T>::operator+(const DenseMatrix<T>& mat) const {
	DenseMatrix<T> result(_size);
	auto size = _size.x * _size.y;
	for (int i = 0; i < size; ++i) {
		(*(result._data))[i] = (*_data)[i] + (*(mat._data))[i];
	}

	return result;
}


template<typename T>
DenseMatrix<T> DenseMatrix<T>::operator-(const DenseMatrix<T>& mat) const {
	DenseMatrix<T> result(_size);
	auto size = _size.x * _size.y;
	for (int i = 0; i < size; ++i) {
		(*(result._data))[i] = (*_data)[i] - (*(mat._data))[i];
	}

	return result;
}


//两个矩阵相乘
template<class T>
DenseMatrix<T> DenseMatrix<T>::operator*(const DenseMatrix<T>& mat)const {
	if (this->_size.y == mat._size.x) {
		DenseMatrix<T> result(this->_size.x, mat._size.y);
		auto newSize = result._size;
		for (int i = 0; i < newSize.x; ++i) {
			for (int j = 0; j < newSize.y; ++j) {
				T tempResult = static_cast<T>(0);
				for (int k = 0; k < this->_size.y; ++k) {
					tempResult += this->lookAt(i, k) * mat.lookAt(k, j);
				}
				result(i, j) = tempResult;
			}
		}

		return result;

	}
}

//矩阵和数相乘
template<class T>
DenseMatrix<T> DenseMatrix<T>::operator*(const T r) const {
	DenseMatrix<T> result(_size);
	auto size = _size.x * _size.y;

	for (int i = 0; i < size; ++i) {
		(*(result._data))[i] = (*_data)[i] * r;
	}

	return result;
}


template<class T>
DenseMatrix<T> operator*(const T r, const DenseMatrix<T>& mat) {
	return mat * r;
}



//矩阵和数相除
template<class T>
DenseMatrix<T> DenseMatrix<T>::operator/(const T r) const {
	DenseMatrix<T> result(_size);
	auto size = _size.x * _size.y;

	for (int i = 0; i < size; ++i) {
		(*(result._data))[i] = (*_data)[i] / r;
	}

	return result;
}

template<class T>
VectorN<T> DenseMatrix<T>::operator*(const VectorN<T>& vec)const {

	VectorN<T> result(vec.dataSize());

	for (int i = 0; i < _size.x; ++i) {
		T sum = static_cast<T>(0.0);
		int count = 0;
		for (int j = 0; j < _size.y; ++j) {
			sum += this->lookAt(i, j) * vec.lookAt(count++);
		}
		result(i) = sum;
	}
	return result;
}


template<class T>
inline DenseMatrix<T> DenseMatrix<T>::identityMatrix(int row, int column) {
	DenseMatrix<T> result(row, column);
	for (int i = 0; i < row; ++i) {
		result(i, i) = static_cast<T>(1.0);
	}
	return result;
}


template<class T>
void DenseMatrix<T>::LU(DenseMatrix<T>& L, DenseMatrix<T>& U)const {

	auto M = DenseMatrix<double>::identityMatrix(_size.x, _size.y);

	for (int i = 0; i < _size.x; ++i) {
		auto dominE = this->lookAt(i, i);
		//如果该行主元不符合条件，那么就做一个行变换
		if (fabs(dominE) < 0.1 || dominE == 0.0) {

		}
		for (int j = 0; j < _size.y; ++j) {

		}
	}
}



#endif