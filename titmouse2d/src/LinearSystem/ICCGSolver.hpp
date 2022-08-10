#pragma once
#include "IterativeSystemSolver.hpp"


template<class T>
class ICCGSolver : public IterativeSystemSolver<T> {
public:
	ICCGSolver() = default;

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

	VectorN<T> preconditioner(const SparseMatrix<T>& L);

private:
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
	for (int i = n - 1; i > -1; --i) {
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
	this->correctResidual(A, x, b);
	auto L = A.ICholesky0();
	auto z = preconditioner(L);
	auto d = z;
	auto& r = this->_r;

	auto delta_new = this->_r * this->_r;
	auto delta0 = delta_new;
	auto minR2 = this->_minR * this->_minR;
	while (delta_new > minR2 * delta0 && this->_iterNum <= this->_maxIterNum) {

		auto Ad = A * d;
		auto rz_old = r * z;
		auto alpha = rz_old / (d * Ad);
		x = x + alpha * d;
		r = r - alpha * Ad;
		z = preconditioner(L);
		auto beta = (r * z) / rz_old;
		d = z + beta * d;
		this->_iterNum++;
		delta_new = r * r;

	}
	std::cout << "迭代次数：" << this->_iterNum << "当前误差：" << this->_r.norm() << std::endl;

}




