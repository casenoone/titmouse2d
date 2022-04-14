#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>

#include <array>

#include <algorithm>

#include "Size2.h"
#include "VectorN.hpp"

#include <omp.h>
//CSRϡ�����ṹ���ص���ÿһ�����ٵ���һ��0

//�˴�CSRϡ�����ĳ�ʼ����ͨ����Ԫ����е�
//���Ȱ��������뵽��Ԫ����
//Ȼ����Ԫ��תΪCSR�ṹ
//��֪��������Ч���Ƿ�ߣ����ֱ�Ӷ�CSR��ֵ���о��ǲ����еģ���Ϊ�ھ�����û��Ԫ�ػ���˵ֻҪ��һ��Ԫ��Ϊ0����ô�Ͷ�λ����Ԫ�ص�λ��
//��˲����ܸ�ָ��λ�ø���ֵ


//����װһ��ϡ������ʱ�򣬲���������ջ��ж��ٷ�0Ԫ��
//���,_data�ĳ����ǲ�ȷ���ģ�����ʹ��vector�ȽϺ���



template<typename T>
class Triplet {
public:
	Triplet();
	Triplet(int _row, int _column, T _value);
	~Triplet();
	void mapToOne(int i, int j);

	bool operator<(const Triplet<T>& t) const;

	bool operator>(const Triplet<T>& t) const;


public:
	int row;
	int column;
	T value;

	int k;
};

template<typename T>
Triplet<T>::Triplet() {

}

template<typename T>
Triplet<T>::~Triplet() {

}

template<typename T>
Triplet<T>::Triplet(int _row, int _column, T _value) :row(_row), column(_column), value(_value) {

}

template<typename T>
void Triplet<T>::mapToOne(int i, int j) {
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
	SparseMatrix();

	SparseMatrix(const int& row, const int& column);

	SparseMatrix(const std::vector<std::vector<T>>& mat);

	void insert(int i, int j, const T r);

	T operator()(int i, int j) const;

	void operator=(const SparseMatrix& mat);

	//�����ҳ�������
	VectorN<T> operator*(const VectorN<T>& vec) const;

	//�������������
	template<class B>
	friend VectorN<B> operator*(const VectorN<T>& vec, const SparseMatrix& mat);

	//�������
	SparseMatrix<T> operator*(const SparseMatrix<T>& mat) const;

	//�����������
	SparseMatrix<T> operator*(const T& r) const;

	//�˵���
	void operator*=(const T& r);

	template<class T>
	friend SparseMatrix<T> operator*(const T& r, const SparseMatrix<T>& mat);

	SparseMatrix<T> operator+(const SparseMatrix<T>& mat) const;

	//������������
	SparseMatrix<T> operator-(const SparseMatrix<T>& mat) const;

	T lookAt(int i, int j)const;

	template<typename Callback>
	void forEachIndex(Callback func) const;

	void build();

	Size2 size()const;

	//�����Ǿ�������
	SparseMatrix<T> inverseForLMatrix() const;

	//�����ĶԽǾ���
	SparseMatrix<T> diagonalMatrix() const;

private:
	//��ȡ����ĳһ�е�Ԫ�ظ���
	T getRowElementNum(int i) const;

private:
	int _row;

	int _column;

	int _valuedNum = 0;

	//����ÿ����Ԫ��λ�õ�ƫ����
	std::shared_ptr<std::vector<int>> _rowIndices;

	//����Ԫ�ض�Ӧ���к�
	std::shared_ptr<std::vector<int>> _columnOffsets;

	//��ž���Ԫ��
	std::shared_ptr<std::vector<T>> _data;

	std::shared_ptr<std::vector<Triplet<T>>> _tempData;

};


template<class T>
SparseMatrix<T>::SparseMatrix() {

}

template<class T>
SparseMatrix<T>::SparseMatrix(const int& row, const int& column) :_row(row), _column(column) {
	_tempData = std::make_shared<std::vector<Triplet<T>>>();
	_rowIndices = std::make_shared<std::vector<int>>();
	_rowIndices->resize(row + 1);

	_columnOffsets = std::make_shared<std::vector<int>>();

	_data = std::make_shared<std::vector<T>>();
}

template<class T>
SparseMatrix<T>::SparseMatrix(const std::vector<std::vector<T>>& mat) :_row(mat.size()), _column(mat[0].size()) {
	_tempData = std::make_shared<std::vector<Triplet<T>>>();
	_rowIndices = std::make_shared<std::vector<int>>();
	_rowIndices->resize(mat.size() + 1);

	_columnOffsets = std::make_shared<std::vector<int>>();
	_data = std::make_shared<std::vector<T>>();

	int sizeX = mat.size();
	int sizeY = mat[0].size();
	for (int i = 0; i < sizeX; ++i) {
		for (int j = 0; j < sizeY; ++j) {
			if (mat[i][j] != 0) {
				this->insert(i, j, mat[i][j]);
			}
		}
	}
	this->build();
}



//���Ƚ�Ԫ�ز�����ʱ����Ԫ���У��ٰ���Ԫ��ת��ΪCSR

template<class T>
void SparseMatrix<T>::insert(int i, int j, const T r) {

	Triplet<T> temp(i, j, r);
	temp.mapToOne(_row, _column);
	_tempData->push_back(temp);
}


template<class T>
void SparseMatrix<T>::build() {
	(*_rowIndices)[_row] = _tempData->size();
	_valuedNum = (*_rowIndices)[_row];
	//����
	sort(_tempData->begin(), _tempData->end());

	_data->resize(_tempData->size());
	_columnOffsets->resize(_tempData->size());

	//װ��CSR�ṹ

	int tempIdx = -100;

	int tempRow = 0;

	//���ȷ�����Ԫ��
	auto size = _tempData->size();
	for (int i = 0; i < size; ++i) {
		if ((*_tempData)[i].row != tempIdx) {
			tempIdx = (*_tempData)[i].row;

			//ƫ����
			(*_rowIndices)[tempRow] = i;
			tempRow += 1;
		}

		(*_columnOffsets)[i] = (*_tempData)[i].column;
		(*_data)[i] = (*_tempData)[i].value;


	}

}


template<class T>
Size2 SparseMatrix<T>::size()const {
	return Size2(_row, _column);
}

//�����Ǿ�������
template<class T>
SparseMatrix<T> SparseMatrix<T>::inverseForLMatrix() const {
	SparseMatrix result(this->_row, this->_column);

	this->forEachIndex([&](int i, int j) {
		if (i == j) {
			result.insert(i, j, this->lookAt(i, j));
		}
		if (i > j) {
			auto L_ii_inv = 1.0 / this->lookAt(i, j);
			T temp = 0.0;
			for (int k = 0; k <= i - 1; ++k) {
				temp += this->lookAt(i, k) * this->lookAt(k, j);
			}
			temp *= -L_ii_inv;
		}
		});

	result.build();
	return result;
}




template<class T>
SparseMatrix<T> SparseMatrix<T>::diagonalMatrix() const {
	SparseMatrix D(this->_row, this->_column);

	for (int i = 0; i < this->_row; ++i) {

		auto value = this->lookAt(i, i);
		if (value == 0)value = 1.0;
		D.insert(i, i, value);
	}

	D.build();

	return D;
}


template<class T>
T SparseMatrix<T>::getRowElementNum(int i) const {
	auto row_offset = (*_rowIndices)[i];
	auto next_row_offset = (*_rowIndices)[i + 1];
	return next_row_offset - row_offset;
}

//ʵ�����
template<class T>
void SparseMatrix<T>::operator=(const SparseMatrix& mat) {

	this->_row = mat._row;
	this->_column = mat._column;
	this->_valuedNum = mat._valuedNum;
	*(this->_rowIndices) = *(mat._rowIndices);
	*(this->_columnOffsets) = *(mat._columnOffsets);
	*(this->_data) = *(mat._data);
	this->_tempData = nullptr;
}



template<class T>
T SparseMatrix<T>::operator()(int i, int j) const {
	if (_valuedNum == 0) {
		return static_cast<T>(0);
	}

	//����ѯ�����к�
	int row = i;
	//������Ԫ�ص�ƫ����
	auto row_offset = (*_rowIndices)[row];
	//��һ����Ԫ�ص�ƫ����
	auto next_row_offset = (*_rowIndices)[row + 1];

	//������в��ǿ���
	if (row_offset != next_row_offset && _valuedNum >= 1) {
		//����ѯ�����к�
		int col = j;
		//������Ԫ�ص��к�
		auto idx = (*_columnOffsets)[row_offset];
		//������Ԫ�صĸ���
		auto row_element_num = next_row_offset - row_offset;

		int count = 0;
		while (count < row_element_num) {
			auto e_idx = row_offset + count;
			if ((*_columnOffsets)[e_idx] == col) {
				return (*_data)[e_idx];
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
T SparseMatrix<T>::lookAt(int i, int j)const {
	return (*this)(i, j);
}

//ע������쳣�ж�

//�����ҳ�������
template<class T>
VectorN<T> SparseMatrix<T>::operator*(const VectorN<T>& vec) const {
	if (this->_column != vec.dataSize()) {
		//�����쳣
	}

	std::vector<T> temp;
	for (int i = 0; i < this->_row; ++i) {
		//��ȡ������Ԫ��ƫ����
		auto row_offset = (*_rowIndices)[i];

		//��ȡ����Ԫ�صĸ���
		auto e_num = getRowElementNum(i);

		T result = static_cast<T>(0);

		int count = 0;
		while (count < e_num) {
			//��ǰԪ�ص��к�
			auto n = row_offset + count;
			auto e_col_num = (*_columnOffsets)[n];

			result += ((*_data)[n] * vec.lookAt(e_col_num));
			count++;
		}

		temp.push_back(result);

	}

	VectorN<T> resultP(temp);
	return resultP;
}





template<class T>
VectorN<T> operator*(const VectorN<T>& vec, const SparseMatrix<T>& mat) {
	std::vector<T> temp;

	auto _column = mat.size().y;

	for (int j = 0; j < _column; ++j) {
		T result = static_cast<T>(0);
		for (int i = 0; i < vec.dataSize(); ++i) {
			result += vec.lookAt(i) * mat(i, j);
		}
		temp.push_back(result);
	}

	VectorN<T> resultVec(temp);
	return resultVec;
}



//�������
template<class T>
SparseMatrix<T> SparseMatrix<T>::operator*(const SparseMatrix<T>& mat) const {
	if (this->_column != mat._row) {
		//�����쳣
	}

	auto resultSize = Size2(this->_row, mat._column);
	SparseMatrix<T> resultMat(resultSize.x, resultSize.y);


	for (int i = 0; i < this->_row; ++i) {
		//��ȡ������Ԫ��ƫ����
		auto row_offset = (*_rowIndices)[i];

		//��ȡ����Ԫ�صĸ���
		auto e_num = getRowElementNum(i);


		for (int j = 0; j < mat._column; ++j) {
			T result = static_cast<T>(0);
			int count = 0;
			while (count < e_num) {
				//��ǰԪ�ص��к�
				auto n = row_offset + count;
				auto e_col_num = (*_columnOffsets)[n];

				result += (*_data)[n] * mat(e_col_num, j);

				count++;
			}
			resultMat.insert(i, j, result);
		}

	}

	resultMat.build();
	return resultMat;
}

//�����������
template<class T>
SparseMatrix<T> SparseMatrix<T>::operator*(const T& r) const {

	SparseMatrix<T> resultMat(this->_row, this->_column);
	resultMat = *this;

	for (int i = 0; i < _valuedNum; ++i) {
		(*(resultMat._data))[i] = (*(this->_data))[i] * r;
	}

	return resultMat;
}

template<class T>
void SparseMatrix<T>::operator*=(const T& r) {
	for (int i = 0; i < _valuedNum; ++i) {
		(*_data)[i] *= r;
	}
}


template<class T>
SparseMatrix<T> operator*(const T& r, const SparseMatrix<T>& mat) {
	return mat * r;
}



//������������
template<class T>
SparseMatrix<T> SparseMatrix<T>::operator+(const SparseMatrix<T>& mat) const {
	SparseMatrix<T> resultMat(this->_row, this->_column);
	this->forEachIndex([&](int i, int j) {
		resultMat.insert(i, j, this->lookAt(i, j) + mat.lookAt(i, j));
		});

	resultMat.build();
	return resultMat;
}


template<class T>
SparseMatrix<T> SparseMatrix<T>::operator-(const SparseMatrix<T>& mat) const {
	SparseMatrix<T> resultMat(this->_row, this->_column);
	this->forEachIndex([&](int i, int j) {
		resultMat.insert(i, j, this->lookAt(i, j) - mat.lookAt(i, j));
		});

	resultMat.build();
	return resultMat;
}


template<typename T>
template<typename Callback>
void SparseMatrix<T>::forEachIndex(Callback func) const {
	for (int i = 0; i < _row; ++i) {
		for (int j = 0; j < _column; ++j) {
			func(i, j);
		}
	}
}



#endif



















