#include <iostream>
using namespace std;


#include "../titmouse2d/src/LinearSystem/SteepestDescentSolver.hpp"
#include "../titmouse2d/src/LinearSystem/ConjugateGradientSolver.hpp"
#include "../titmouse2d/src/LinearSystem/JacobiSolver.hpp"
#include "../titmouse2d/src/LinearSystem/GaussSeidelSolver.hpp"
#include "../titmouse2d/src/LinearSystem/RelaxedJacobiSolver.hpp"
#include "../titmouse2d/src/LinearSystem/SORsolver.hpp"
#include "../titmouse2d/src/LinearSystem/RelaxedJacobiSolver.hpp"
#include "../titmouse2d/src/Matrix2x2.hpp"
#include "../titmouse2d/src/Matrix3x3.hpp"
#include "../titmouse2d/src/DenseMatrix.hpp"

#include "../titmouse2d/src/Eulerian/VertexCenteredScalarGrid2.h"


#include <array>

//插入排序
//时间复杂度：O(n2)
void insert(double a[], int len) {
	int temp;
	for (int i = 1; i < len; ++i) {
		temp = a[i];
		int j = i - 1;
		for (; j >= 0 && a[j] > temp; --j) {
			a[j + 1] = a[j];
		}
		a[j + 1] = temp;
	}

}


//选择排序
void choice(double a[], int len) {
	for (int i = 0; i < len; ++i) {
		auto min_index = i;
		for (int j = i + 1; j < len; ++j) {
			if (a[j] < a[min_index]) {
				min_index = j;
			}
		}
		std::swap(a[i], a[min_index]);
	}
}


//冒泡排序

void bubbleSort(double a[], int len) {
	bool exchange = false;
	for (int i = len - 1; i >= 0; --i) {
		for (int j = 0; j < i; ++j) {
			if (a[j] > a[j + 1]) {
				std::swap(a[j], a[j + 1]);
				exchange = true;
			}
		}
		if (exchange == false) {
			break;
		}
	}
}

void shellHelp(double a[], int start, int& gap, int len) {
	for (int i = start + gap; i < len; i += gap) {
		auto pos = i;
		auto currentValue = a[i];

		while (pos > start && a[pos - gap] > currentValue) {
			a[pos] = a[pos - gap];
			pos = pos - gap;
		}
		a[pos] = currentValue;
	}
}

//希尔排序
void shellSort(double a[], int len) {

	int gap = len / 2;
	while (gap > 0) {
		for (int i = 0; i < gap; ++i) {
			shellHelp(a, i, gap, len);
		}
		gap = gap / 2;
	}

}


int main() {


	double a[] = { 5,3,1,2,9,6 ,4 };
	int len = 7;

	shellSort(a, len);

	for (int i = 0; i < len; ++i) {
		cout << a[i] << endl;
	}


	SteepestDescentSolver<double> SteepSolver;

	ConjugateGradientSolver <double> cgSolver;

	JacobiSolver <double> jSolver;

	GaussSeidelSolver <double> gaussSolver;

	SORSolver<double> sorSolver(1.67);

	RelaxedJacobiSolver<double> rjbSolver(0.5);

	vector<vector<double>> A_data = {
		{10.0, 1.0, 2.0,3.0,4.0},
		{1.0, 9.0,-1.0,2.0,-3.0},
		{2.0,-1.0,7.0,3.0,-5},
		{3.0,2.0,3.0,12.0,-1.0},
		{4.0,-3.0,-5.0,-1.0,15.0}
	};

	/*vector<vector<
		double>> A_data = {
			{4.,-1.,0.},
			{-1.,4.,-1.},
			{0.,-1.,4.}
	};*/

	SparseMatrix<double> A(A_data);



	vector<double> b_used = { 12,-27,14,-17,12 };
	//vector<double> b_used = { 1.,4.,-3. };
	vector<double> x_used;
	x_used.resize(5);
	VectorN<double> b(b_used);
	VectorN<double> x(x_used);





	//SteepSolver.compute(A, x, b);
	//cgSolver.compute(A, x, b);
	//jSolver.compute(A, x, b);
	//gaussSolver.compute(A, x, b);
	//sorSolver.compute(A, x, b);
	rjbSolver.compute(A, x, b);

	x.forEachIndex([&](int i) {
		//cout << x[i] << endl;
		});

	//该算例的正确答案： 1.000000 -2.000000  3.000000 -2.000000  1.000000


	//该算例的正确答案： 0.499999 1  -0.49999 

	Matrix2x2<double> mat2(1, 2, -3, 0);
	auto k = mat2.inverse();

	Matrix3x3<double> mat3(Vector3D(1, -3, 2), Vector3D(5, 1, -1), Vector3D(2, -1, 4));

	auto mat3Invers = mat3.inverse();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			//cout << mat3Invers(i, j) << endl;
		}
	}



	vector<vector<
		double>> dense1 = {
			{4.,-1.,0.},
			{-1.,4.,-1.},
			{0.,-1.,4.}
	};

	vector<vector<
		double>> dense2 = {
			{2.,1.,0.},
			{5.,-1.,0.},
			{3.,9.,7.}
	};

	DenseMatrix<double> mat41(3, 3, dense1);
	DenseMatrix<double> mat42(3, 3, dense2);
	vector<double> _b = { 1.,4.,-3. };

	VectorN<double> mat_b(_b);

	auto mat_result = mat41 * mat42 * mat_b;
	for (int i = 0; i < 3; ++i) {

		//cout << mat_result[i] << endl;

	}

	auto identity = DenseMatrix<double>::identityMatrix(6, 6);

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			//cout << identity(i, j) << endl;
		}
	}



	return 0;
}