#ifndef STEEPESTDESCENTSOLVER_HPP
#define STEEPESTDESCENTSOLVER_HPP

#include "IterativeSystemSolver.h"
#include "../SparseMatrix.hpp"
#include "../VectorN.hpp"

template<class T>
class SteepestDescentSolver :public IterativeSystemSolver {
public:
	SteepestDescentSolver();
	~SteepestDescentSolver();

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b)  const;

	//�����������С���
	void setMinError(T minValue);

private:

	//����residual
	void correctResidual();

	//����residual
	void computResidual();

	//���㲽��ϵ��
	void computAlpha();

private:
	//���
	VectorN<T> _r;

	//����ϵ��
	T alpha;

	//��С���
	T _minR = 1e-5;
	
};


template<class T>
SteepestDescentSolver<T>::SteepestDescentSolver() {

}

template<class T>
SteepestDescentSolver<T>::~SteepestDescentSolver() {

}

template<class T>
void SteepestDescentSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b)  const {

}

template<class T>
void SteepestDescentSolver<T>::setMinError(T minValue) {
	_minR = minValue;
}

//����residual
template<class T>
void SteepestDescentSolver<T>::correctResidual() {

}


//����residual
template<class T>
void SteepestDescentSolver<T>::computResidual() {

}


//���㲽��ϵ��
template<class T>
void SteepestDescentSolver<T>::computAlpha() {

}

#endif