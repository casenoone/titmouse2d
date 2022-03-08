#ifndef DIRECTSOLVER_HPP
#define DIRECTSOLVER_HPP

//直接法求解线性系统，主要用于稠密矩阵求解
//擦，实在是写不动了，暂时不写了，直接上eigen
#include <iostream>

#include "../DenseMatrix.hpp"

template<class T>
class DirectSolver {
public:
	virtual ~DirectSolver() = 0;

	virtual void compute(
		const DenseMatrix<T>& A,
		VectorN<T>& x,
		const VectorN<T>& b) = 0;

};

#endif