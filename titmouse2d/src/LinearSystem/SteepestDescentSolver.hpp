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

	//设置容许的最小误差
	void setMinError(T minValue);

private:

	//矫正residual
	void correctResidual();

	//计算residual
	void computResidual();

	//计算步进系数
	void computAlpha();

private:
	//误差
	VectorN<T> _r;

	//步进系数
	T alpha;

	//最小误差
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

//矫正residual
template<class T>
void SteepestDescentSolver<T>::correctResidual() {

}


//计算residual
template<class T>
void SteepestDescentSolver<T>::computResidual() {

}


//计算步进系数
template<class T>
void SteepestDescentSolver<T>::computAlpha() {

}

#endif