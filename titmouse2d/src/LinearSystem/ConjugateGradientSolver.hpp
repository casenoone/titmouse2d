#ifndef CONJUGATEGRADIENTSOLVER_HPP
#define CONJUGATEGRADIENTSOLVER_HPP

#include "IterativeSystemSolver.h"
#include "../SparseMatrixPtr.hpp"
#include "../VectorNPtr.hpp"

template<class T>
class ConjugateGradientSolver : public IterativeSystemSolver {
public:
	ConjugateGradientSolver();
	~ConjugateGradientSolver();

	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

	//设置容许的最小误差
	//这个函数写到基类里
	void setMinError(T minValue);

private:

	//矫正residual
	void correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);
	
	//计算residual
	void computResidual(const SparseMatrixPtr<T>& A);

	//计算步进系数
	void computeAlpha(const SparseMatrixPtr<T>& A);

	void computeBeta();

	void computeD();

private:
	//误差
	VectorNPtr<T> _r;

	VectorNPtr<T> _r1;

	//步进系数
	T _alpha;

	T _beta;

	VectorNPtr<T> _d;

	//最小误差
	T _minR = 1e-5;

	//记录迭代次数
	size_t _iterNum = 0;

	size_t _maxIterNum = 500;


};


template<class T>
ConjugateGradientSolver<T>::ConjugateGradientSolver() {

}

template<class T>
ConjugateGradientSolver<T>::~ConjugateGradientSolver() {

}

template<class T>
void ConjugateGradientSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	correctResidual(A, x, b);
	_d = _r;

	auto delta_new = _r * _r;
	auto delta0 = delta_new;
	auto delta_old = 0.0;
	auto minR2 = _minR * _minR;

	while (delta_new > minR2*delta0 && _iterNum <= _maxIterNum) {
		computeAlpha(A);
		x = x + _alpha * _d;
		if (_iterNum+1 % 50 == 0) {
			correctResidual(A, x, b);
			_r1 = _r;
		}
		else {
			computResidual(A);
		}

		if (_r1.norm() <= 0) {
			//cout << "迭代次数：" << _iterNum << "当前误差：" << _r.norm() << endl;
			break;
		}

		delta_old = delta_new;
		delta_new = _r1 * _r1;

		computeBeta();
		computeD();

		_r = _r1;
		_iterNum++;
		
	}
	cout << "迭代次数：" << _iterNum << "当前误差：" << _r.norm() << endl;
}

//设置容许的最小误差
template<class T>
void ConjugateGradientSolver<T>::setMinError(T minValue) {
	_minR = minValue;
}


//矫正residual
template<class T>
void ConjugateGradientSolver<T>::correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	_r = b - A * x;
}

//计算residual
template<class T>
void ConjugateGradientSolver<T>::computResidual(const SparseMatrixPtr<T>& A) {
	_r1 = _r - _alpha * A * _d;
}

//计算步进系数
template<class T>
void ConjugateGradientSolver<T>::computeAlpha(const SparseMatrixPtr<T>& A) {
	_alpha = (_r * _r) / (_d * A * _d);
}

template<class T>
void ConjugateGradientSolver<T>::computeBeta() {
	_beta = (_r1 * _r1) / (_r * _r);
}


template<class T>
void ConjugateGradientSolver<T>::computeD() {
	_d = _r1 + _beta * _d;
}

#endif