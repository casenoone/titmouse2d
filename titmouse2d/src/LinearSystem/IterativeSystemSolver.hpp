#ifndef ITERATIVESYSTEMSOLVER_HPP
#define ITERATIVESYSTEMSOLVER_HPP

#include "../SparseMatrixPtr.hpp"
#include "../VectorNPtr.hpp"

template <class T>
class IterativeSystemSolver {
public:
	IterativeSystemSolver();
	
	virtual ~IterativeSystemSolver();

	virtual void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) = 0;

	//�����������С���
	void setMinError(T minValue);

	//����residual
	void correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

protected:
	//���
	VectorNPtr<T> _r;

	//��С���
	T _minR = 1e-5;

	//��¼��������
	size_t _iterNum = 0;

	//����������
	size_t _maxIterNum = 500;
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
void IterativeSystemSolver<T>::correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	this->_r = b - A * x;
}

#endif