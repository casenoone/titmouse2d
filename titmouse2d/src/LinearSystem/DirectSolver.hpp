#ifndef DIRECTSOLVER_HPP
#define DIRECTSOLVER_HPP

//ֱ�ӷ��������ϵͳ����Ҫ���ڳ��ܾ������
//����ʵ����д�����ˣ���ʱ��д�ˣ�ֱ����eigen
#include <iostream>

#include "../DenseMatrix.hpp"

template<class T>
class DirectSolver {
public:
	virtual ~DirectSolver() = 0;

	virtual void compute(
		const DenseMatrix<T>& A,
		VectorN<T>& x,
		const VectorN<T>& b) = 0;

};

#endif