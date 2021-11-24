#ifndef CONJUGATEGRADIENTSOLVER_HPP
#define CONJUGATEGRADIENTSOLVER_HPP

#include "IterativeSystemSolver.hpp"


template<class T>
class ConjugateGradientSolver : public IterativeSystemSolver<T> {
public:
	ConjugateGradientSolver();
	~ConjugateGradientSolver();

	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

private:
		
	//计算residual
	void computResidual(const SparseMatrixPtr<T>& A);

	//计算步进系数
	void computeAlpha(const SparseMatrixPtr<T>& A);

	void computeBeta();

	void computeD();

private:
	

	VectorNPtr<T> _r1;

	//步进系数
	T _alpha;

	T _beta;

	VectorNPtr<T> _d;

};


template<class T>
ConjugateGradientSolver<T>::ConjugateGradientSolver() {

}

template<class T>
ConjugateGradientSolver<T>::~ConjugateGradientSolver() {

}

template<class T>
void ConjugateGradientSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	this->correctResidual(A, x, b);
	_d = this->_r;

	auto delta_new = this->_r * this->_r;
	auto delta0 = delta_new;
	auto delta_old = 0.0;
	auto minR2 = this->_minR * this->_minR;

	while (delta_new > minR2*delta0 && this->_iterNum <= this->_maxIterNum) {
		computeAlpha(A);
		x = x + _alpha * _d;
		if (this->_iterNum+1 % 50 == 0) {
			this->correctResidual(A, x, b);
			_r1 = this->_r;
		}
		else {
			computResidual(A);
		}

		if (_r1.norm() <= 0) {
			//cout << "迭代次数：" << this->_iterNum << "当前误差：" << this->_r.norm() << endl;
			break;
		}

		delta_old = delta_new;
		delta_new = _r1 * _r1;

		computeBeta();
		computeD();

		this->_r = _r1;
		this->_iterNum++;
		
	}
	//cout << this->_minR << endl;
	//cout << "迭代次数：" << this->_iterNum << "当前误差：" << this->_r.norm() << endl;
}


//计算residual
template<class T>
void ConjugateGradientSolver<T>::computResidual(const SparseMatrixPtr<T>& A) {
	_r1 = this->_r - _alpha * A * _d;
}

//计算步进系数
template<class T>
void ConjugateGradientSolver<T>::computeAlpha(const SparseMatrixPtr<T>& A) {
	_alpha = (this->_r * this->_r) / (_d * A * _d);
}

template<class T>
void ConjugateGradientSolver<T>::computeBeta() {
	_beta = (_r1 * _r1) / (this->_r * this->_r);
}


template<class T>
void ConjugateGradientSolver<T>::computeD() {
	_d = _r1 + _beta * _d;
}

#endif