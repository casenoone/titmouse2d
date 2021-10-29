#ifndef STEEPESTDESCENTSOLVER_HPP
#define STEEPESTDESCENTSOLVER_HPP

#include "IterativeSystemSolver.h"
#include "../SparseMatrixPtr.hpp"
#include "../VectorNPtr.hpp"


template<class T>
class SteepestDescentSolver :public IterativeSystemSolver {
public:
	SteepestDescentSolver();
	~SteepestDescentSolver();

	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b)  ;

	//�����������С���
	void setMinError(T minValue);

private:

	//����residual
	void correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

	//����residual
	void computResidual(const SparseMatrixPtr<T>& A);

	//���㲽��ϵ��
	void computAlpha(const SparseMatrixPtr<T>& A);

private:
	//���
	VectorNPtr<T> _r;

	//����ϵ��
	T _alpha;

	//��С���
	T _minR = 1e-5;

	//��¼��������
	size_t _iterNum = 0;
	
};


template<class T>
SteepestDescentSolver<T>::SteepestDescentSolver() {

}

template<class T>
SteepestDescentSolver<T>::~SteepestDescentSolver() {

}

template<class T>
void SteepestDescentSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b)   {
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
			cout << "����������" << _iterNum << "��ǰ��" << _r.norm() << endl;
			break;
		}
		computAlpha(A);
		x = x + _alpha * _r;
		_iterNum++;
		
	}
	cout << "����������" << _iterNum << "��ǰ��" << _r.norm() << endl;
	
}

template<class T>
void SteepestDescentSolver<T>::setMinError(T minValue) {
	_minR = minValue;
}

//����residual
template<class T>
void SteepestDescentSolver<T>::correctResidual(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	_r = b - A * x;
}


//����residual
template<class T>
void SteepestDescentSolver<T>::computResidual(const SparseMatrixPtr<T>& A) {
	_r = _r - _alpha * A * _r;
}


//���㲽��ϵ��
template<class T>
void SteepestDescentSolver<T>::computAlpha(const SparseMatrixPtr<T>& A) {
	_alpha = _r * _r / (_r * A * _r);
}

#endif