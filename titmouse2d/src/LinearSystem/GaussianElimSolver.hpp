#ifndef GAUSSIANELIMSOLVER_HPP
#define GAUSSIANELIMSOLVER_HPP

#include "DirectSolver.hpp"

template<class T>
class GaussianElimSolver : public DirectSolver<T> {
public:
	GaussianElimSolver() = default;
	virtual void compute(
		const DenseMatrix<T>& A,
		VectorN<T>& x,
		const VectorN<T>& b) override;
};

template<class T>
void GaussianElimSolver<T>::compute(
	const DenseMatrix<T>& A,
	VectorN<T>& x,
	const VectorN<T>& b) {

}

#endif