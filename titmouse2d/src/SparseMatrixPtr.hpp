#ifndef SPARSEMATRIXPTR_HPP
#define SPARSEMATRIXPTR_HPP

#include "SparseMatrix.hpp"
#include "VectorNPtr.hpp"

template<class T>
class SparseMatrixPtr {
public:

	friend class VectorNPtr<T>;

	SparseMatrixPtr();

	SparseMatrixPtr(const size_t& row, const size_t& column);

	SparseMatrixPtr(const vector<vector<T>>& mat);

	SparseMatrixPtr(shared_ptr<SparseMatrix<T>> p);

	virtual ~SparseMatrixPtr();

	void insert(size_t i, size_t j, const T r);

	size_t getRowElementNum(size_t i) const;

	T operator()(size_t i, size_t j) const;

	void operator=(const SparseMatrixPtr& mat);

	//矩阵右乘列向量
	VectorNPtr<T> operator*(const VectorNPtr<T>& vec) const;

	//矩阵左乘行向量
	template<class B>
	friend VectorNPtr<B> operator*(const VectorNPtr<T>& vec, const SparseMatrixPtr& mat);

	//矩阵相乘
	SparseMatrixPtr<T> operator*(const SparseMatrixPtr<T>& mat) const;

	//矩阵与数相乘
	SparseMatrixPtr<T> operator*(const T& r) const;

	template<class T>
	friend SparseMatrixPtr<T> operator*(const T& r, const SparseMatrixPtr<T>& mat);

	SparseMatrixPtr<T> operator+(const SparseMatrixPtr<T>& mat) const;

	SparseMatrixPtr<T> operator-(const SparseMatrixPtr<T>& mat) const;

	T lookAt(size_t i, size_t j)const;

	template<typename Callback>
	void forEachIndex(Callback func) const;

	void build();

	Size2 size()const;

	SparseMatrixPtr<T> inverseForLMatrix() const;

	//求矩阵的对角矩阵
	SparseMatrixPtr<T> diagonalMatrix() const;

private:
	shared_ptr<SparseMatrix<T>> sparseMatrixPtr;
};



template<class T>
SparseMatrixPtr<T>::SparseMatrixPtr() {

}

template<class T>
SparseMatrixPtr<T>::SparseMatrixPtr(const size_t& row, const size_t& column) {
	sparseMatrixPtr = make_shared<SparseMatrix<T>>(row, column);
}

template<class T>
SparseMatrixPtr<T>::SparseMatrixPtr(const vector<vector<T>>& mat) {
	sparseMatrixPtr = make_shared<SparseMatrix<T>>(mat);
}

template<class T>
SparseMatrixPtr<T>::SparseMatrixPtr(shared_ptr<SparseMatrix<T>> p) {
	sparseMatrixPtr = p;
}


template<class T>
SparseMatrixPtr<T>::~SparseMatrixPtr() {

}

//首先将元素插入临时的三元组中，再把三元组转换为CSR

template<class T>
void SparseMatrixPtr<T>::insert(size_t i, size_t j, const T r) {

	sparseMatrixPtr->insert(i, j, r);
}


template<class T>
void SparseMatrixPtr<T>::build() {
	
	sparseMatrixPtr->build();
}

template<class T>
Size2 SparseMatrixPtr<T>::size()const {
	return sparseMatrixPtr->size();
}

template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::inverseForLMatrix() const {
	SparseMatrixPtr result(sparseMatrixPtr->inverseForLMatrix());
	return result;
}

template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::diagonalMatrix() const {
	SparseMatrixPtr result(sparseMatrixPtr->diagonalMatrix());
	return result;
}


template<class T>
size_t SparseMatrixPtr<T>::getRowElementNum(size_t i) const {
	return sparseMatrixPtr->getRowElementNum(i);
}

template<class T>
void SparseMatrixPtr<T>::operator=(const SparseMatrixPtr& mat) {

	(*sparseMatrixPtr) = (*mat);
}



template<class T>
T SparseMatrixPtr<T>::operator()(size_t i, size_t j) const {
	return (*sparseMatrixPtr)(i, j);
}


template<class T>
T SparseMatrixPtr<T>::lookAt(size_t i, size_t j)const {
	return (*sparseMatrixPtr)(i, j);
}

//注意添加异常判断

//矩阵右乘列向量
template<class T>
VectorNPtr<T> SparseMatrixPtr<T>::operator*(const VectorNPtr<T>& vec) const {
	VectorNPtr<T> result((*sparseMatrixPtr) * (*(vec.getVectorNPtr())));
	return result;
}





template<class T>
VectorNPtr<T> operator*(const VectorNPtr<T>& vec, const SparseMatrixPtr<T>& mat) {
	VectorNPtr<T> result(mat * vec);
	return result;
}



//矩阵相乘
template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::operator*(const SparseMatrixPtr<T>& mat) const {
	SparseMatrixPtr<T> result((*sparseMatrixPtr) * (*(mat.sparseMatrixPtr)));
	return result;
}

//矩阵与数相乘
template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::operator*(const T& r) const {
	SparseMatrixPtr<T> result((*sparseMatrixPtr) * r);
	return result;
}

template<class T>
SparseMatrixPtr<T> operator*(const T& r, const SparseMatrixPtr<T>& mat) {
	return mat * r;
}


template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::operator+(const SparseMatrixPtr<T>& mat) const {
	SparseMatrixPtr<T> result(*(this->sparseMatrixPtr) + (*(mat.sparseMatrixPtr)));
	return result;
}


template<class T>
SparseMatrixPtr<T> SparseMatrixPtr<T>::operator-(const SparseMatrixPtr<T>& mat) const {
	SparseMatrixPtr<T> result(*(this->sparseMatrixPtr) - (*(mat.sparseMatrixPtr)));
	return result;
}


template<typename T>
template<typename Callback>
void SparseMatrixPtr<T>::forEachIndex(Callback func) const {
	
	auto _row = sparseMatrixPtr->size().x;
	auto _column = sparseMatrixPtr->size().y;

	for (size_t i = 0; i < _row; ++i) {
		for (size_t j = 0; j < _column; ++j) {
			func(i, j);
		}
	}
}



#endif