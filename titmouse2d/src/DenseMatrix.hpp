#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
using namespace std;

#include <vector>

#include "VectorN.hpp"
#include "Size2.h"
#include "ConstVar.h"

//对于矩阵来说，底层数据用array是不是比vector更为合适？
//是的，从逻辑上来讲，矩阵的大小一经确定，就不允许更改尺寸了
//Array2不一定，或许自适应网格需要这样的结构？
//而对于Array类型来说，长度时经常变的，因此用Vector比较好

//应该写一个Matrix基类
//稀疏矩阵+稠密矩阵

//考虑行优先与列优先

//我们规定，一个矩阵的大小至少是2 x 2，1 x n或n x 1使用VectorN

//对于矩阵行列的要求，交给使用者去判断
//例如两个矩阵相乘，要事先指定结果矩阵的

//矩阵默认是列优先存储，那么使用向量的指针的好处在于，可以非常方便的转换行列存储而无需大量内存操作


//定义行优先和列优先
const int ROW = 0;
const int COLUMN = 1;

template<class T>
class DenseMatrix {
public:

	template<typename T>
	using DenseMatrixPtr = shared_ptr<DenseMatrix<T>>;


	DenseMatrix();
	~DenseMatrix();

	DenseMatrix(size_t row, size_t column, const vector<vector<T>>& data);

	DenseMatrix(const DenseMatrix<T>& other);

	Size2 dataSize() const;

	void reSize(Size2 newSize);

	template<typename Callback>
	void forEachIndex(Callback func) const;

	//设置行优先、列优先
	void setOrder(int state);

	//返回矩阵是行优先还是列优先
	int getOrder() const;

	T& operator()(size_t i, size_t j);

	T& lookAt(size_t i, size_t j) const;

	//两个矩阵相加
	DenseMatrixPtr<T> operator+(const DenseMatrix<T>& mat) const;

	//两个矩阵相减
	DenseMatrixPtr<T> operator-(const DenseMatrix<T>& mat) const;

	//两个矩阵相乘
	T& operator*(const DenseMatrix<T>& mat)const;

	//矩阵和数相乘
	DenseMatrixPtr<T> operator*(const T r) const;

private:
	Size2 _size;

	//这里考虑使用array类型
	vector<VectorXPtr<T>> _data;

	//默认为列优先存储
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
DenseMatrix<T>::DenseMatrix(size_t row, size_t column, const vector<vector<T>>& data) {

	_size = Size2(row, column);

	for (int j = 0; j < column; ++j) {

		vector<T> temp;

		for (int i = 0; i < row; ++i) {
			temp.push_back(data[i][j]);
		}

		VectorXPtr<T> temp_p = make_shared<VectorX<T>>(temp);
		_data.push_back(temp_p);

	}

}


//这里要检查一下两个矩阵的尺寸是否相同
//需要一些故障检测的东西，目前先不用
template<class T>
DenseMatrix<T>::DenseMatrix(const DenseMatrix<T>& other) {
	_data = other._data;
}

template<class T>
Size2 DenseMatrix<T>::dataSize() const {
	return _size;
}

//想了一下，还是不提供这个方法了
//对于一个矩阵来说，一旦被确定，就不允许再修改其尺寸了
//既然有修改尺寸的需求，那何不新声明一个矩阵
template<class T>
void DenseMatrix<T>::reSize(Size2 newSize) {

}

template<typename T>
template<typename Callback>
void DenseMatrix<T>::forEachIndex(Callback func) const {
	for (size_t i = 0; i < _size.x; ++i) {
		for (size_t j = 0; j < _size.y; ++j) {
			func(i, j);
		}
	}
}

template<typename T>
//设置行优先、列优先
void DenseMatrix<T>::setOrder(int state) {
	_order = state;
}


template<typename T>
//返回矩阵是行优先还是列优先
int DenseMatrix<T>::getOrder() const {
	return _order;
}

template<typename T>
T& DenseMatrix<T>::lookAt(size_t i, size_t j) const{
	auto result = 0;

	//如果是列优先
	if (_order == ROW) {
		return (*_data[j])[i];
	}
	//如果是行优先
	else {
		return (*_data[i])[j];
	}
}

template<typename T>
T& DenseMatrix<T>::operator()(size_t i, size_t j) {
	auto result = 0;
	
	//如果是列优先
	if (_order == COLUMN) {
		return (*_data[j])[i];
	}
	//如果是行优先
	else {
		return (*_data[i])[j];
	}
}


//两个矩阵相加
//这里也要注意加一个异常检测，当两个矩阵尺寸不同时无法相加
template<typename T>
DenseMatrixPtr<T> DenseMatrix<T>::operator+(const DenseMatrix<T>& mat) const {
	if (mat._size == this->_size) {

		vector<vector<T>> temp(_size.x, vector<T>(_size.y));
		
		this->forEachIndex([&](size_t i,size_t j) {
		
			temp[i][j] = this->lookAt(i, j) + mat.lookAt(i, j);

		});
		
		DenseMatrixPtr<T> result = make_shared<DenseMatrix<T>>(_size.x, _size.y, temp);
		return result;

	}
}

//两个矩阵相减
//DenseMatrixPtr<T> operator-(const DenseMatrix<T> mat) const;

//两个矩阵相乘
//T& operator*(const DenseMatrix<T> mat)const;

//矩阵和数相乘
//DenseMatrixPtr<T> operator*(const T r) const;

#endif