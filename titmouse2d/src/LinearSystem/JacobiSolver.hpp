#ifndef JACOBISOLVER_HPP
#define JACOBISOLVER_HPP

#include "IterativeSystemSolver.hpp"
#include "../SparseMatrix.hpp"
#include "../VectorN.hpp"

//这里可以进一步优化内存
//暂时不管了，用到的时候再优化


const double eps = 2.718281828459;

template<class T>
class JacobiSolver : public IterativeSystemSolver<T> {
public:
	JacobiSolver();
	~JacobiSolver();

	//求解Ax = b
	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

	SparseMatrix<T> computeD(const SparseMatrix<T>& A) const;


};


template<class T>
JacobiSolver<T>::JacobiSolver() {

}


template<class T>
JacobiSolver<T>::~JacobiSolver() {

}

template<class T>
SparseMatrix<T> JacobiSolver<T>::computeD(const SparseMatrix<T>& A) const {
	SparseMatrix<T> D(A.size().x, A.size().y);

	for (int i = 0; i < A.size().x; ++i) {

		auto value = A.lookAt(i, i);
		if (value == 0)value = 1.0;
		D.insert(i, i, value);
	}

	D.build();

	return D;
}




template<class T>
void JacobiSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	this->_r = (b - A * x);
	auto err = this->_r.norm();
	auto relerr = err;
	while (this->_iterNum <= this->_maxIterNum) {
		auto D = computeD(A);

		auto R = A - D;

		SparseMatrix<T> D_inv(A.size().x, A.size().y);
		for (int i = 0; i < A.size().x; ++i) {
			int j = i;
			auto value = A.lookAt(i, j);
			if (value == 0)value = 1.0;

			D_inv.insert(i, j, 1 / value);
		}

		D_inv.build();

		x = D_inv * (b - R * x);

		err = (b - A * x).norm();
		if (err <= this->_minR) {
			break;
		}
		this->_iterNum++;


	}
}


#endif