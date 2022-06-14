#pragma once
#include "IterativeSystemSolver.hpp"


//��ʵ��һ���򵥵�JacobiԤ�������ݶȷ�
template<class T>
class JacobiPCGSolver : public IterativeSystemSolver<T> {
public:
	JacobiPCGSolver() = default;

	void compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b);

private:
	VectorN<T> _r1;

	//����ϵ��
	T _alpha;

	T _beta;

	VectorN<T> _d;

};



template<class T>
void JacobiPCGSolver<T>::compute(const SparseMatrix<T>& A, VectorN<T>& x, const VectorN<T>& b) {
	this->_maxIterNum = 50;

	this->correctResidual(A, x, b);
	auto delta_new = this->_r * this->_r;
	auto delta0 = delta_new;
	auto minR2 = this->_minR * this->_minR;

	//���ȣ�ѡȡԤ�������M
	//������JacobiԤ�������M����A�����Ӧ�ĶԽǾ���
	auto M = A.diagonalMatrix();
	auto r0 = b;
	VectorN<T> r0_hat(A.size().x);
	VectorN<T> r1_hat(A.size().x);

	//�ɲ���
	for (int i = 0; i < A.size().x; ++i) {
		r0_hat[i] = r0[i] / M.lookAt(i, i);
	}

	auto p0 = r0_hat;
	VectorN<T> p1(A.size().x);
	while (this->_iterNum <= this->_maxIterNum
		&& delta_new > minR2 * delta0) {
		double alpha = 0.0;
		alpha = (this->_r * r0_hat) / (p0 * A * p0);
		x = x + alpha * p0;
		this->_r1 = this->_r - alpha * A * p0;

		for (int i = 0; i < A.size().x; ++i) {
			r1_hat[i] = this->_r1[i] / M.lookAt(i, i);
		}

		double beta = (this->_r1 * r1_hat) / (this->_r * r0_hat);
		p1 = r1_hat + beta * p0;

		this->_r = this->_r1;
		p0 = p1;
		r0_hat = r1_hat;

		delta_new = this->_r * this->_r;

		this->_iterNum++;
	}
}




