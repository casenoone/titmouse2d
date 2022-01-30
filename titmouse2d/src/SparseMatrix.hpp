#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
using namespace std;

#include <array>

#include <algorithm>

#include "Size2.h"
#include "VectorN.hpp"

#include <omp.h>
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
	void mapToOne(size_t i, size_t j);

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
Triplet<T>::Triplet(size_t _row, size_t _column, T _value) :row(_row), column(_column), value(_value) {

}

template<typename T>
void Triplet<T>::mapToOne(size_t i, size_t j) {
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

template<class T>
class SparseMatrix {
public:

	template<typename T>
	using sparseMatrixPtr = shared_ptr<SparseMatrix<T>>;

	SparseMatrix();

	SparseMatrix(const size_t& row, const size_t& column);

	SparseMatrix(const vector<vector<T>>& mat);

	virtual ~SparseMatrix();



	void insert(size_t i, size_t j, const T r);

	T operator()(size_t i, size_t j) const;

	void operator=(const SparseMatrix& mat);

	//矩阵右乘列向量
	vectorNPtr<T> operator*(const VectorN<T>& vec) const;

	//矩阵左乘行向量
	template<class B>
	friend vectorNPtr<B> operator*(const VectorN<T>& vec, const SparseMatrix& mat);

	//矩阵相乘
	sparseMatrixPtr<T> operator*(const SparseMatrix<T>& mat) const;

	//矩阵与数相乘
	sparseMatrixPtr<T> operator*(const T& r) const;

	//乘等于
	void operator*=(const T& r);

	template<class T>
	friend sparseMatrixPtr<T> operator*(const T& r, const SparseMatrix<T>& mat);

	sparseMatrixPtr<T> operator+(const SparseMatrix<T>& mat) const;

	//矩阵与矩阵相减
	sparseMatrixPtr<T> operator-(const SparseMatrix<T>& mat) const;

	T lookAt(size_t i, size_t j)const;

	template<typename Callback>
	void forEachIndex(Callback func) const;

	void build();

	Size2 size()const;

	//下三角矩阵求逆
	sparseMatrixPtr<T> inverseForLMatrix() const;

	//求矩阵的对角矩阵
	sparseMatrixPtr<T> diagonalMatrix() const;

private:
	//获取矩阵某一行的元素个数
	T getRowElementNum(size_t i) const;

private:
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
SparseMatrix<T>::SparseMatrix(const size_t& row, const size_t& column) :_row(row), _column(column) {
	_tempData = new vector<Triplet<T>>();
	_rowIndices.resize(row + 1);

}

template<class T>
SparseMatrix<T>::SparseMatrix(const vector<vector<T>>& mat) :_row(mat.size()), _column(mat[0].size()) {
	_tempData = new vector<Triplet<T>>();
	_rowIndices.resize(mat.size() + 1);

	size_t sizeX = mat.size();
	size_t sizeY = mat[0].size();
#pragma omp parallel for num_threads(20) 
	for (size_t i = 0; i < sizeX; ++i) {
		for (size_t j = 0; j < sizeY; ++j) {
			if (mat[i][j] != 0) {
				this->insert(i, j, mat[i][j]);
			}
		}
	}
	this->build();
}


template<class T>
SparseMatrix<T>::~SparseMatrix() {

}

//首先将元素插入临时的三元组中，再把三元组转换为CSR

template<class T>
void SparseMatrix<T>::insert(size_t i, size_t j, const T r) {

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

	int tempRow = 0;

	//首先放置首元素
	auto size = _tempData->size();
	//#pragma omp parallel for num_threads(20) reduction(+:tempRow)
	for (int i = 0; i < size; ++i) {
		if ((*_tempData)[i].row != tempIdx) {
			tempIdx = (*_tempData)[i].row;

			//偏移量
			_rowIndices[tempRow] = i;
			tempRow += 1;
		}

		_columnOffsets[i] = (*_tempData)[i].column;
		_data[i] = (*_tempData)[i].value;


	}

	delete _tempData;

}


template<class T>
Size2 SparseMatrix<T>::size()const {
	return Size2(_row, _column);
}

//下三角矩阵求逆
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::inverseForLMatrix() const {
	sparseMatrixPtr<T> result = make_shared<SparseMatrix>(this->_row, this->_column);

	this->forEachIndex([&](size_t i, size_t j) {
		if (i == j) {
			result->insert(i, j, this->lookAt(i, j));
		}
		if (i > j) {
			auto L_ii_inv = 1.0 / this->lookAt(i, j);
			T temp = 0.0;
			for (size_t k = 0; k <= i - 1; ++k) {
				temp += this->lookAt(i, k) * this->lookAt(k, j);
			}
			temp *= -L_ii_inv;
		}
		});

	result->build();
	return result;
}




template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::diagonalMatrix() const {
	sparseMatrixPtr<T> D = make_shared<SparseMatrix>(this->_row, this->_column);

	for (size_t i = 0; i < this->_row; ++i) {

		auto value = this->lookAt(i, i);
		if (value == 0)value = 1.0;
		D->insert(i, i, value);
	}

	D->build();

	return D;
}


template<class T>
T SparseMatrix<T>::getRowElementNum(size_t i) const {
	auto row_offset = _rowIndices[i];
	auto next_row_offset = _rowIndices[i + 1];
	return next_row_offset - row_offset;
}

template<class T>
void SparseMatrix<T>::operator=(const SparseMatrix& mat) {

	this->_row = mat._row;
	this->_column = mat._column;
	this->_valuedNum = mat._valuedNum;
	this->_rowIndices = mat._rowIndices;
	this->_columnOffsets = mat._columnOffsets;
	this->_data = mat._data;
	this->_tempData = nullptr;
}



template<class T>
T SparseMatrix<T>::operator()(size_t i, size_t j) const {
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
	return (*this)(i, j);
}

//注意添加异常判断

//矩阵右乘列向量
template<class T>
vectorNPtr<T> SparseMatrix<T>::operator*(const VectorN<T>& vec) const {
	if (this->_column != vec.dataSize()) {
		//引发异常
	}

	vector<T> temp;
	for (size_t i = 0; i < this->_row; ++i) {
		//获取该行首元素偏移量
		auto row_offset = _rowIndices[i];

		//获取该行元素的个数
		auto e_num = getRowElementNum(i);

		T result = static_cast<T>(0);

		size_t count = 0;
		while (count < e_num) {
			//当前元素的列号
			auto n = row_offset + count;
			auto e_col_num = _columnOffsets[n];

			result += (_data[n] * vec.lookAt(e_col_num));
			count++;
		}

		temp.push_back(result);

	}

	auto resultP = make_shared<VectorN<T>>(temp);
	return resultP;
}





template<class T>
vectorNPtr<T> operator*(const VectorN<T>& vec, const SparseMatrix<T>& mat) {
	vector<T> temp;

	auto _column = mat.size().y;

	for (size_t j = 0; j < _column; ++j) {
		T result = static_cast<T>(0);
		for (int i = 0; i < vec.dataSize(); ++i) {
			result += vec.lookAt(i) * mat(i, j);
		}
		temp.push_back(result);
	}

	auto resultVec = make_shared<VectorN<T>>(temp);
	return resultVec;
}



//矩阵相乘
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator*(const SparseMatrix<T>& mat) const {
	if (this->_column != mat._row) {
		//引发异常
	}

	auto resultSize = Size2(this->_row, mat._column);
	auto  resultMat = make_shared<SparseMatrix<T>>(resultSize.x, resultSize.y);


	for (size_t i = 0; i < this->_row; ++i) {
		//获取该行首元素偏移量
		auto row_offset = _rowIndices[i];

		//获取该行元素的个数
		auto e_num = getRowElementNum(i);


		for (size_t j = 0; j < mat._column; ++j) {
			T result = static_cast<T>(0);
			size_t count = 0;
			while (count < e_num) {
				//当前元素的列号
				auto n = row_offset + count;
				auto e_col_num = _columnOffsets[n];

				result += _data[n] * mat(e_col_num, j);

				count++;
			}
			resultMat->insert(i, j, result);
		}

	}

	resultMat->build();
	return resultMat;
}

//矩阵与数相乘
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator*(const T& r) const {

	auto resultMat = make_shared<SparseMatrix<T>>(this->_row, this->_column);

	(*resultMat) = (*this);

	for (size_t i = 0; i < _valuedNum; ++i) {
		resultMat->_data[i] *= r;
	}

	return resultMat;
}

template<class T>
void SparseMatrix<T>::operator*=(const T& r) {
	for (size_t i = 0; i < _valuedNum; ++i) {
		this->_data[i] *= r;
	}
}


template<class T>
sparseMatrixPtr<T> operator*(const T& r, const SparseMatrix<T>& mat) {
	return (*this) * r;
}



//矩阵与矩阵相减
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator+(const SparseMatrix<T>& mat) const {
	auto resultMat = make_shared<SparseMatrix<T>>(this->_row, this->_column);
	this->forEachIndex([&](size_t i, size_t j) {
		resultMat->insert(i, j, this->lookAt(i, j) + mat.lookAt(i, j));
		});

	resultMat->build();
	return resultMat;
}


template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator-(const SparseMatrix<T>& mat) const {
	auto resultMat = make_shared<SparseMatrix<T>>(this->_row, this->_column);
	this->forEachIndex([&](size_t i, size_t j) {
		resultMat->insert(i, j, this->lookAt(i, j) - mat.lookAt(i, j));
		});

	resultMat->build();
	return resultMat;
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



















