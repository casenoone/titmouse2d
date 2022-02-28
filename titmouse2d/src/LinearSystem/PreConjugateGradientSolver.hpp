#ifndef PREGAUSSSEIDELSOLVER_HPP
#define PREGAUSSSEIDELSOLVER_HPP

#include "IterativeSystemSolver.hpp"

template<class T>
class PcgSolver : public IterativeSystemSolver<T> {
public:
	PcgSolver() = default;

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

private:
	VectorN<T> _r1;

	//步进系数
	T _alpha;

	T _beta;

	VectorN<T> _d;

};



template<class T>
void PcgSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	this->correctResidual(A, x, b);
}




#endif