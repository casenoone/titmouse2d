#ifndef GAUSSSEIDELSOLVER_HPP
#define GAUSSSEIDELSOLVER_HPP

#include "IterativeSystemSolver.hpp"

template<class T>
class GaussSeidelSolver : public IterativeSystemSolver<T> {
public:
	GaussSeidelSolver() = default;

	//求解Ax = b
	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

};


//这里的计算采用分量形式，不用矩阵格式
//因为矩阵格式要写LU分解，懒得写了
template<class T>
void GaussSeidelSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
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


		x = x1;
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