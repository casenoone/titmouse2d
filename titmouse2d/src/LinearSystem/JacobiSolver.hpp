#ifndef JACOBISOLVER_HPP
#define JACOBISOLVER_HPP

#include "IterativeSystemSolver.h"
#include "../SparseMatrix.hpp"
#include "../VectorN.hpp"

template<class T>
class JacobiSolver : public IterativeSystemSolver {
public:
	JacobiSolver();
	~JacobiSolver();

	//«ÛΩ‚Ax = b
	void compute(const SparseMatrix<T>& A, VectorY<T>& x, const VectorY<T>& b)  const;
};


template<class T>
JacobiSolver<T>::JacobiSolver() {

}


template<class T>
JacobiSolver<T>::~JacobiSolver() {

}

template<class T>
void JacobiSolver<T>::compute(const SparseMatrix<T>& A, VectorY<T>& x, const VectorY<T>& b) const {

}


#endif