#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
using namespace std;

#include <array>

#include <algorithm>

#include "Size2.h"

//CSR稀疏矩阵结构的特点是每一行至少得有一个0

//此处CSR稀疏矩阵的初始化是通过三元组进行的
//首先把数据输入到三元组中
//然后将三元组转为CSR结构
//不知道这样做效率是否高，如果直接对CSR赋值，感觉是不可行的，因为在矩阵中没有元素或者说只要有一行元素为0，那么就定位不到元素的位置
//因此不可能给指定位置赋初值


//在组装一个稀疏矩阵的时候，并不清楚最终会有多少非0元素
//因此,_data的长度是不确定的，所以使用vector比较合适




template<typename T>
class Triplet {
public:
	Triplet();
	Triplet(size_t _row, size_t _column, T _value);
	~Triplet();
	void mapToOne(size_t i,size_t j);

	bool operator<(const Triplet<T>& t) const;

	bool operator>(const Triplet<T>& t) const;

public:
	size_t row;
	size_t column;
	T value;

	size_t k;
};

template<typename T>
Triplet<T>::Triplet() {

}

template<typename T>
Triplet<T>::~Triplet() {

}

template<typename T>
Triplet<T>::Triplet(size_t _row, size_t _column, T _value):row(_row),column(_column),value(_value) {
	
}

template<typename T>
void Triplet<T>::mapToOne(size_t i,size_t j) {
	k = row * j + j;
}

template<typename T>
bool Triplet<T>::operator<(const Triplet<T>& t) const {
	if (this->k < t.k) {
		return true;
	}
	else {
		return false;
	}
}


template<typename T>
bool Triplet<T>::operator>(const Triplet<T>& t) const {
	if (this->k > t.k) {
		return true;
	}
	else {
		return false;
	}
}

//暂时不考虑矩阵的对称存储
template<class T>
class SparseMatrix {
public:
	SparseMatrix();

	SparseMatrix(const size_t& row, const size_t& column);

	SparseMatrix(const vector<vector<T>>& mat);

	virtual ~SparseMatrix();

	void insert(size_t i, size_t j, const T r);

	T operator()(size_t i, size_t j);

	T lookAt(size_t i, size_t j)const;

	template<typename Callback>
	void forEachIndex(Callback func) const;

	void build();

public :
	size_t _row;

	size_t _column;

	size_t _valuedNum = 0;

	//放置每行首元素位置的偏移量
	vector<size_t> _rowIndices;

	//矩阵元素对应的列号
	vector<size_t> _columnOffsets;

	//存放矩阵元素
	vector<T> _data;

	vector<Triplet<T>>* _tempData;

};

template<typename T>
using sparseMatrixPtr = shared_ptr<SparseMatrix<T>>;





template<class T>
SparseMatrix<T>::SparseMatrix() {

}

template<class T>
SparseMatrix<T>::SparseMatrix(const size_t& row, const size_t& column):_row(row),_column(column) {
	_tempData = new vector<Triplet<T>>();
	_rowIndices.resize(row + 1);

}

template<class T>
SparseMatrix<T>::SparseMatrix(const vector<vector<T>>& mat) {

}


template<class T>
SparseMatrix<T>::~SparseMatrix() {

}

//首先将元素插入临时的三元组中，再把三元组转换为CSR

template<class T>
void SparseMatrix<T>::insert(size_t i,size_t j, const T r) {

	Triplet<T> temp(i, j, r);
	temp.mapToOne(_row, _column);
	_tempData->push_back(temp);
}


template<class T>
void SparseMatrix<T>::build() {
	_rowIndices[_row] = _tempData->size();
	_valuedNum = _rowIndices[_row];
	//排序
	sort(_tempData->begin(), _tempData->end());

	_data.resize(_tempData->size());
	_columnOffsets.resize(_tempData->size());

	//装入CSR结构

	size_t tempIdx = -100;

	size_t tempRow = 0;

	//首先放置首元素
	auto size = _tempData->size();
	for (size_t i = 0; i < size; ++i) {
		if ((*_tempData)[i].row != tempIdx) {
			tempIdx = (*_tempData)[i].row;
			
			//偏移量
			_rowIndices[tempRow] = i;
			tempRow++;
		}

		_columnOffsets[i] = (*_tempData)[i].column;
		_data[i] = (*_tempData)[i].value;

		
	}

	delete _tempData;

}


template<class T>
T SparseMatrix<T>::operator()(size_t i, size_t j) {
	if (_valuedNum == 0) {
		return static_cast<T>(0);
	}


	//待查询处的行号
	size_t row = i;
	//该行首元素的偏移量
	auto row_offset = _rowIndices[row];
	//下一行首元素的偏移量
	auto next_row_offset = _rowIndices[row + 1];

	//cout << "i:" << i << "," << j << "," << "value:" << row_offset << endl;


	//如果该行不是空行
	if (row_offset != next_row_offset && _valuedNum >= 1) {
		//待查询处的列号
		size_t col = j;
		//该行首元素的列号
		auto idx = _columnOffsets[row_offset];
		//该行中元素的个数
		auto row_element_num = next_row_offset - row_offset;
		//cout << "i:" << i << "," << j << "," << "value:" << row_offset << "," << next_row_offset << endl;
		size_t count = 0;
		while (count < row_element_num) {
			auto e_idx = row_offset + count;
			if (_columnOffsets[e_idx] == col) {
				return _data[e_idx];
			}
			count++;
		}

		return static_cast<T>(0);
	}
	else {
		return static_cast<T>(0);
	}
}


template<class T>
T SparseMatrix<T>::lookAt(size_t i, size_t j)const {
	if (_valuedNum == 0) {
		return static_cast<T>(0);
	}

	//待查询处的行号
	size_t row = i;
	//该行首元素的偏移量
	auto row_offset = _rowIndices[row];
	//下一行首元素的偏移量
	auto next_row_offset = _rowIndices[row + 1];

	//cout << "i:" << i << "," << j << "," << "value:" << row_offset << endl;


	//如果该行不是空行
	if (row_offset != next_row_offset && _valuedNum>=1) {
		//待查询处的列号
		size_t col = j;
		//该行首元素的列号
		auto idx = _columnOffsets[row_offset];
		//该行中元素的个数
		auto row_element_num = next_row_offset - row_offset;

		size_t count = 0;
		while (count < row_element_num) {
			auto e_idx = row_offset + count;
			if (_columnOffsets[e_idx] == col) {
				return _data[e_idx];
			}
			count++;
		}

		return static_cast<T>(0);
	}
	else {
		return static_cast<T>(0);
	}
}




template<typename T>
template<typename Callback>
void SparseMatrix<T>::forEachIndex(Callback func) const {
	for (size_t i = 0; i < _row; ++i) {
		for (size_t j = 0; j < _column; ++j) {
			func(i, j);
		}
	}
}



#endif