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

	//设置容许的最小误差
	void setMinError(T minValue);

	//矫正residual
	void correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

protected:
	//误差
	VectorNPtr<T> _r;

	//最小误差
	T _minR = 1e-7;

	//记录迭代次数
	size_t _iterNum = 0;

	//最大迭代次数
	size_t _maxIterNum = 1000;
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