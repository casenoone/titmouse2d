#ifndef ITERATIVESYSTEMSOLVER_HPP
#define ITERATIVESYSTEMSOLVER_HPP

#include "../SparseMatrix.hpp"
#include "../VectorN.hpp"

template <class T>
class IterativeSystemSolver {
public:
	IterativeSystemSolver();

	virtual ~IterativeSystemSolver();

	virtual void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) = 0;

	//�����������С���
	void setMinError(T minValue);

	//����residual
	void correctResidual(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

protected:
	//���
	VectorN<T> _r;

	//��С���
	T _minR = 1e-7;

	//��¼��������
	int _iterNum = 0;

	//����������
	int _maxIterNum = 300;
};


template<class T>
IterativeSystemSolver<T>::IterativeSystemSolver() {

}



template<class T>
IterativeSystemSolver<T>::~IterativeSystemSolver() {

}


template<class T>
void IterativeSystemSolver<T>::setMinError(T minValue) {
	_minR = minValue;
}


template<class T>
void IterativeSystemSolver<T>::correctResidual(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	this->_r = b - A * x;
}

#endif