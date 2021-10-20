#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
using namespace std;

#include <vector>

//Ӧ��дһ��Matrix����
//ϡ�����+���ܾ���

//������������������

//���ǹ涨��һ������Ĵ�С������2 x 2��1 x n��n x 1ʹ��VectorN

//���ھ������е�Ҫ�󣬽���ʹ����ȥ�ж�
//��������������ˣ�Ҫ����ָ����������

//����Ĭ���������ȴ洢����ôʹ��������ָ��ĺô����ڣ����Էǳ������ת�����д洢����������ڴ����






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