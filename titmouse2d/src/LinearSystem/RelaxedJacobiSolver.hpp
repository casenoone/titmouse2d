#ifndef RELAXEDJACOBI_HPP
#define RELAXEDJACOBI_HPP

#include "JacobiSolver.hpp"


template <class T>
class RelaxedJacobiSolver : public JacobiSolver<T> {
public:
	RelaxedJacobiSolver() = default;

	RelaxedJacobiSolver(T omega) :_omega(omega) {}

	//求解Ax = b
	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

private:
	//松弛因子
	T _omega = 1.67;

};

template<class T>
void RelaxedJacobiSolver<T>::compute(const SparseMatrixPtr<T>& A,
	VectorNPtr<T>& x,
	const VectorNPtr<T>& b) {

	this->_r = (b - A * x);
	auto err = this->_r.norm();
	auto relerr = err;
	while (this->_iterNum <= this->_maxIterNum) {
		auto D = this->computeD(A);


		auto R = A - D;

		SparseMatrixPtr<T> D_inv(A.size().x, A.size().y);
		for (size_t i = 0; i < A.size().x; ++i) {
			size_t j = i;
			auto value = A.lookAt(i, j);
			if (value == 0)value = 1.0;

			D_inv.insert(i, j, 1 / value);
		}

		D_inv.build();

		//x = D_inv * (b - R * x);

		x = (1 - _omega) * x + _omega * D_inv * (b - R * x);

		err = (b - A * x).norm();
		if (err <= this->_minR) {
			break;
		}
		this->_iterNum++;


	}

}



#endif