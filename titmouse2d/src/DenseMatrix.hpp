#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
using namespace std;

#include <vector>

//应该写一个Matrix基类
//稀疏矩阵+稠密矩阵

//考虑行优先与列优先

//我们规定，一个矩阵的大小至少是2 x 2，1 x n或n x 1使用VectorN

//对于矩阵行列的要求，交给使用者去判断
//例如两个矩阵相乘，要事先指定结果矩阵的

//矩阵默认是列优先存储，那么使用向量的指针的好处在于，可以非常方便的转换行列存储而无需大量内存操作






template<class T>
class DenseMatrix {
public:
	DenseMatrix();
	~DenseMatrix();

	DenseMatrix(size_t row, size_t column, const vector<vector<T>>& data);

	DenseMatrix(const Matrix& other);

	Size2 dataSize() const;

	void reSize(Size2 newSize);

	template<typename Callback>
	void forEachIndex(Callback func) const;



private:
	vector<vector<T>> _data;

	Size2 _size;



};

template<typename T>
using DenseMatrixPtr = shared_ptr<DenseMatrix<T>>;


#endif