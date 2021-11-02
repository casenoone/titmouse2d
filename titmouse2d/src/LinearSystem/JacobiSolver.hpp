#ifndef JACOBISOLVER_HPP
#define JACOBISOLVER_HPP

#include "IterativeSystemSolver.hpp"
#include "../SparseMatrixPtr.hpp"
#include "../VectorNPtr.hpp"

//这里可以进一步优化内存
//暂时不管了，用到的时候再优化


const double eps = 2.718281828459;

template<class T>
class JacobiSolver : public IterativeSystemSolver<T>{
public:
	JacobiSolver();
	~JacobiSolver();

	//求解Ax = b
	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

	SparseMatrixPtr<T> computeD(const SparseMatrixPtr<T>& A) const;

	
};


template<class T>
JacobiSolver<T>::JacobiSolver() {

}


template<class T>
JacobiSolver<T>::~JacobiSolver() {

}

template<class T>
SparseMatrixPtr<T> JacobiSolver<T>::computeD(const SparseMatrixPtr<T>& A) const {
	SparseMatrixPtr<T> D(A.size().x, A.size().y);

	for (size_t i = 0; i < A.size().x; ++i) {

		auto value = A.lookAt(i, i);
		if (value == 0)value = 1.0;
		D.insert(i, i, value);
	}

	D.build();

	return D;
}






//目标矩阵是A
//将A分解为D和R
//其中D为对角矩阵
template<class T>
void JacobiSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	this->_r = (b - A * x);
	auto err = this->_r.norm();
	auto relerr = err;
	while (this->_iterNum <= this->_maxIterNum) {
		auto D = computeD(A);
		
		
		auto R = A - D;

		SparseMatrixPtr<T> D_inv(A.size().x, A.size().y);
		for (size_t i = 0; i < A.size().x; ++i) {
			size_t j = i;
			auto value = A.lookAt(i, j);
			if (value == 0)value = 1.0;

			D_inv.insert(i, j, 1 / value);
		}

		D_inv.build();

		x = D_inv * (b - R * x);
		
		err = (b - A * x).norm();
		if (err <= this->_minR) {
			break;
		}
		this->_iterNum++;

		//cout << x[0] << endl;

	}
	cout << "迭代次数：" << this->_iterNum << "当前误差：" << err << endl;
}


#endif