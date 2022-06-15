#pragma once
#include "IterativeSystemSolver.hpp"


template<class T>
class GSPCGSolver : public IterativeSystemSolver<T> {
public:
	GSPCGSolver() = default;

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

private:
	VectorN<T> _r1;
};



template<class T>
void GSPCGSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {

}




