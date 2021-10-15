#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
using namespace std;

#include <vector>

//应该写一个Matrix基类
//稀疏矩阵+稠密矩阵

//考虑行优先与列优先

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



#endif