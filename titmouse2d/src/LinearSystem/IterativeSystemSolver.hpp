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

	//设置容许的最小误差
	void setMinError(T minValue);

	//矫正residual
	void correctResidual(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

protected:
	//误差
	VectorN<T> _r;

	//最小误差
	T _minR = 1e-7;

	//记录迭代次数
	int _iterNum = 0;

	//最大迭代次数
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