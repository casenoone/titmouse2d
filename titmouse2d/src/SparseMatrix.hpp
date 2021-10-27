#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>
using namespace std;

#include <array>

#include <algorithm>

#include "Size2.h"

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
T SparseMatrix<T>::operator()(size_t i, size_t j) {
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

	//����ѯ�����к�
	size_t row = i;
	//������Ԫ�ص�ƫ����
	auto row_offset = _rowIndices[row];
	//��һ����Ԫ�ص�ƫ����
	auto next_row_offset = _rowIndices[row + 1];

	//cout << "i:" << i << "," << j << "," << "value:" << row_offset << endl;


	//������в��ǿ���
	if (row_offset != next_row_offset && _valuedNum>=1) {
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