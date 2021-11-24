#ifndef STEEPESTDESCENTSOLVER_HPP
#define STEEPESTDESCENTSOLVER_HPP

#include "IterativeSystemSolver.hpp"


template<class T>
class SteepestDescentSolver : public IterativeSystemSolver<T> {
public:
	SteepestDescentSolver();
	~SteepestDescentSolver();

	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);


private:

	//����residual
	void computeResidual(const SparseMatrixPtr<T>& A);

	//���㲽��ϵ��
	void computeAlpha(const SparseMatrixPtr<T>& A);

private:
	
	//����ϵ��
	T _alpha;

};


template<class T>
SteepestDescentSolver<T>::SteepestDescentSolver() {

}

template<class T>
SteepestDescentSolver<T>::~SteepestDescentSolver() {

}

template<class T>
void SteepestDescentSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b)   {
	this->correctResidual(A, x, b);
	while (this->_r.norm() > this->_minR && this->_iterNum <= this->_maxIterNum) {
		if ((this->_iterNum+1) % 50 == 0) {
			this->correctResidual(A, x, b);
		}
		else {
			computeResidual(A);
		}
		if (this->_r.norm() <= 0) {
			
			break;
		}
		computeAlpha(A);
		x = x + _alpha * this->_r;
		this->_iterNum++;
		
	}

	//cout << "����������" << this->_iterNum << "��ǰ��" << this->_r.norm() << endl;
	
}



//����residual
template<class T>
void SteepestDescentSolver<T>::computeResidual(const SparseMatrixPtr<T>& A) {
	this->_r = this->_r - _alpha * A * this->_r;
}


//���㲽��ϵ��
template<class T>
void SteepestDescentSolver<T>::computeAlpha(const SparseMatrixPtr<T>& A) {
	_alpha = this->_r * this->_r / (this->_r * A * this->_r);
}

#endif