#ifndef JACOBISOLVER_HPP
#define JACOBISOLVER_HPP

#include "IterativeSystemSolver.h"
#include "../SparseMatrixPtr.hpp"
#include "../VectorNPtr.hpp"

//������Խ�һ���Ż��ڴ�
//��ʱ�����ˣ��õ���ʱ�����Ż�


const double eps = 2.718281828459;

template<class T>
class JacobiSolver : public IterativeSystemSolver {
public:
	JacobiSolver();
	~JacobiSolver();

	//���Ax = b
	void compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b);

	SparseMatrixPtr<T> computeD(const SparseMatrixPtr<T>& A) const;



private:
	//���
	VectorNPtr<T> _r;

	//��С���
	T _minR = 1e-5;

	//��¼��������
	size_t _iterNum = 0;

	size_t _maxIterNum = 500;
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






//Ŀ�������A
//��A�ֽ�ΪD��R
//����DΪ�ԽǾ���
template<class T>
void JacobiSolver<T>::compute(const SparseMatrixPtr<T>& A, VectorNPtr<T>& x, const VectorNPtr<T>& b) {
	_r = (b - A * x);
	auto err = _r.norm();
	auto relerr = err;
	while (_iterNum <=_maxIterNum) {
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
		if (err <= _minR) {
			break;
		}
		_iterNum++;

		//cout << x[0] << endl;

	}
	cout << "����������" << _iterNum << "��ǰ��" << err << endl;
}


#endif