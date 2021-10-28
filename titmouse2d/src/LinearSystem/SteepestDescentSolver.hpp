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

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b)  ;

	//设置容许的最小误差
	void setMinError(T minValue);

private:

	//矫正residual
	void correctResidual(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

	//计算residual
	void computResidual(const SparseMatrix<T>& A);

	//计算步进系数
	void computAlpha(const SparseMatrix<T>& A);

private:
	//误差
	VectorN<T> _r;

	//步进系数
	T _alpha;

	//最小误差
	T _minR = 1e-5;

	//记录迭代次数
	size_t _iterNum = 0;
	
};


template<class T>
SteepestDescentSolver<T>::SteepestDescentSolver() {

}

template<class T>
SteepestDescentSolver<T>::~SteepestDescentSolver() {

}

template<class T>
void SteepestDescentSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b)   {
	correctResidual(A, x, b);
	while (_r.norm() > _minR) {
		if (_iterNum == 0 || _iterNum % 5 == 0) {
			correctResidual(A, x, b);
		}
		else {
			//correctResidual(A, x, b);
			computResidual(A);
		}
		if (_r.norm() <= 0) {
			cout << "迭代次数：" << _iterNum << "当前误差：" << _r.norm() << endl;
			break;
		}
		computAlpha(A);
		x = *(x + (*(_alpha * _r)));
		_iterNum++;
		
	}
	cout << "迭代次数：" << _iterNum << "当前误差：" << _r.norm() << endl;
	
}

template<class T>
void SteepestDescentSolver<T>::setMinError(T minValue) {
	_minR = minValue;
}

//矫正residual
template<class T>
void SteepestDescentSolver<T>::correctResidual(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	_r = *(b - *(A * x));
}


//计算residual
template<class T>
void SteepestDescentSolver<T>::computResidual(const SparseMatrix<T>& A) {
	_r = *(_r - (*(_alpha * (*(A * _r)))));
}


//计算步进系数
template<class T>
void SteepestDescentSolver<T>::computAlpha(const SparseMatrix<T>& A) {
	_alpha = (_r * _r) / (*(_r * A) * _r);	
}

#endif