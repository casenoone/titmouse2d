#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
using namespace std;

#include <array>

#include <algorithm>

#include "Size2.h"
#include "VectorN.hpp"
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

//��ʱ�����Ǿ���ĶԳƴ洢
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
	
	//�����ҳ�������
	VectorNPtr<T> operator*(const VectorN<T>& vec) const;

	//�������������
	template<class B>
	friend VectorNPtr<B> operator*(const VectorN<T>& vec, const SparseMatrix& mat);

	//�������
	sparseMatrixPtr<T> operator*(const SparseMatrix<T>& mat) const;

	//�����������
	sparseMatrixPtr<T> operator*(const T& r) const;

	template<class T>
	friend sparseMatrixPtr<T> operator*(const T& r, const SparseMatrix<T>& mat);
	
	T lookAt(size_t i, size_t j)const;

	template<typename Callback>
	void forEachIndex(Callback func) const;

	void build();

private:
	//��ȡ����ĳһ�е�Ԫ�ظ���
	T getRowElementNum(size_t i) const;

public :
	size_t _row;

	size_t _column;

	size_t _valuedNum = 0;

	//����ÿ����Ԫ��λ�õ�ƫ����
	vector<size_t> _rowIndices;

	//����Ԫ�ض�Ӧ���к�
	vector<size_t> _columnOffsets;

	//��ž���Ԫ��
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

//���Ƚ�Ԫ�ز�����ʱ����Ԫ���У��ٰ���Ԫ��ת��ΪCSR

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
	//����
	sort(_tempData->begin(), _tempData->end());

	_data.resize(_tempData->size());
	_columnOffsets.resize(_tempData->size());

	//װ��CSR�ṹ

	size_t tempIdx = -100;

	size_t tempRow = 0;

	//���ȷ�����Ԫ��
	auto size = _tempData->size();
	for (size_t i = 0; i < size; ++i) {
		if ((*_tempData)[i].row != tempIdx) {
			tempIdx = (*_tempData)[i].row;
			
			//ƫ����
			_rowIndices[tempRow] = i;
			tempRow++;
		}

		_columnOffsets[i] = (*_tempData)[i].column;
		_data[i] = (*_tempData)[i].value;

		
	}

	delete _tempData;

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
T SparseMatrix<T>::operator()(size_t i, size_t j) const{
	if (_valuedNum == 0) {
		return static_cast<T>(0);
	}

	//����ѯ�����к�
	size_t row = i;
	//������Ԫ�ص�ƫ����
	auto row_offset = _rowIndices[row];
	//��һ����Ԫ�ص�ƫ����
	auto next_row_offset = _rowIndices[row + 1];

	//cout << "i:" << i << "," << j << "," << "value:" << row_offset << endl;


	//������в��ǿ���
	if (row_offset != next_row_offset && _valuedNum >= 1) {
		//����ѯ�����к�
		size_t col = j;
		//������Ԫ�ص��к�
		auto idx = _columnOffsets[row_offset];
		//������Ԫ�صĸ���
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

//ע�������쳣�ж�

//�����ҳ�������
template<class T>
VectorNPtr<T> SparseMatrix<T>::operator*(const VectorN<T>& vec) const {
	if (this->_column != vec.dataSize()) {
		//�����쳣
	}

	vector<T> temp;
	for (size_t i = 0; i < this->_row; ++i) {
		//��ȡ������Ԫ��ƫ����
		auto row_offset = _rowIndices[i];
		
		//��ȡ����Ԫ�صĸ���
		auto e_num = getRowElementNum(i);

		T result = static_cast<T>(0);

		size_t count = 0;
		while (count < e_num) {
			//��ǰԪ�ص��к�
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
VectorNPtr<T> operator*(const VectorN<T>& vec, const SparseMatrix<T>& mat){
	vector<T> temp;
    
	for (size_t j = 0; j < mat._column; ++j) {
		T result = static_cast<T>(0);
		for(int i = 0; i<vec.dataSize(); ++i){
			result += vec.lookAt(i) * mat(i, j);
		}
		temp.push_back(result);
	}

	auto resultVec = make_shared<VectorN<T>>(temp);
	return resultVec;
}



//�������
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator*(const SparseMatrix<T>& mat) const {
	if (this->_column != mat._row) {
		//�����쳣
	}

	auto resultSize = Size2(this->_row, mat._column);
	auto  resultMat = make_shared<SparseMatrix<T>>(resultSize.x, resultSize.y);
	

	for (size_t i = 0; i < this->_row; ++i) {
		//��ȡ������Ԫ��ƫ����
		auto row_offset = _rowIndices[i];

		//��ȡ����Ԫ�صĸ���
		auto e_num = getRowElementNum(i);
		

		for (size_t j = 0; j < mat._column; ++j) {
			T result = static_cast<T>(0);
			size_t count = 0;
			while (count < e_num) {
				//��ǰԪ�ص��к�
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

//�����������
template<class T>
sparseMatrixPtr<T> SparseMatrix<T>::operator*(const T& r) const {
	
	auto resultMat = make_shared<SparseMatrix<T>>(this->_row, this->_column);
	
	(*resultMat) = (*this);
	
	for (size_t i= 0; i < _valuedNum; ++i) {
		resultMat->_data[i] *= r;
	}

	return resultMat;
}

template<class T>
sparseMatrixPtr<T> operator*(const T& r,const SparseMatrix<T>& mat) {
	return (*this) * r;
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