#ifndef SORSOLVER_HPP
#define SORSOLVER_HPP

#include "IterativeSystemSolver.hpp"

template <class T>
class SORSolver : public IterativeSystemSolver<T> {
public:
	SORSolver() = default;

	SORSolver(T omega) :_omega(omega) {}

	//求解Ax = b
	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

private:
	//松弛因子
	T _omega = 1.67;

};

template<class T>
void SORSolver<T>::compute(const SparseMatrix<T>& A,
	VectorN<T>& x,
	const VectorN<T>& b) {

	this->_r = (b - A * x);
	auto x1 = x;
	auto err = this->_r.norm();
	while (this->_iterNum <= this->_maxIterNum) {

		//int 是无符号数
		x1.forEachIndex([&](int i) {
			T term1 = 0.0;
			T term2 = 0.0;
			for (int j = 0; j + 1 <= i; ++j) {

				term1 += A.lookAt(i, j) * x1[j];
			}

			for (int j = i + 1; j < A.size().y; ++j) {
				term2 += A.lookAt(i, j) * x[j];
			}


			x1[i] = (1.0 / A.lookAt(i, i)) * (b.lookAt(i) - term1 - term2);


			});


		x = _omega * x1 + (1 - _omega) * x;
		this->_r = (b - A * x);
		err = this->_r.norm();

		if (err <= this->_minR) {

			break;
		}
		this->_iterNum++;

	}
	//cout << "迭代次数：" << this->_iterNum << "当前误差：" << this->_r.norm() << endl;
}


#endif