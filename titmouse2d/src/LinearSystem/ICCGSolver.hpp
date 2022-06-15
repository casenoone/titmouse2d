#pragma once
#include "IterativeSystemSolver.hpp"


template<class T>
class ICCGSolver : public IterativeSystemSolver<T> {
public:
	ICCGSolver() = default;

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

	VectorN<T> preconditioner(const SparseMatrix<T>& L);

private:
	VectorN<T> _r;
	VectorN<T> _r1;
};


template<class T>
VectorN<T> ICCGSolver<T>::preconditioner(const SparseMatrix<T>& L) {
	int n = L.size().x;
	VectorN<T> resultTemp(n);

	//前向消去
	for (int i = 0; i < n; ++i) {
		resultTemp[i] = this->_r[i] / L(i, i);
		for (int j = 0; j < i; ++j) {
			resultTemp[i] -= L(i, j) / L(i, i) * resultTemp[j];
		}
	}


	VectorN<T> result(n);
	//后向消去
	for (int i = n - 1, i > -1; --i) {
		result[i] = resultTemp[i] / L(i, i);
		for (int j = i + 1; j < n; ++j) {
			result[i] -= L(j, i) / L(i, i) * result[j];
		}
	}
	return result;
}

template<class T>
void ICCGSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	VectorN<T>  p;
}




