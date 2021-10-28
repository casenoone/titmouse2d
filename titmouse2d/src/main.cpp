#include <iostream>
using namespace std;

#include "Vector2.hpp"
#include "Array.hpp"
#include "Array2.hpp"
#include "VectorN.hpp"
#include "DenseMatrix.hpp"
#include "SparseMatrix.hpp"
#include "LinearSystem/SteepestDescentSolver.hpp"

#include <array>

int main() {
	
	
	SteepestDescentSolver<double> iterSolver;

	SparseMatrix<double> A(5, 5);
	A.insert(0,0,10);
	A.insert(0,1,1);
	A.insert(0,2,2);
	A.insert(0,3,3);
	A.insert(0,4,4);
	A.insert(1,0,1);
	A.insert(1,1,9);
	A.insert(1,2,-1);
	A.insert(1,3,2);
	A.insert(1,4,-3);
	A.insert(2,0,2);
	A.insert(2,1,-1);
	A.insert(2,2,7);
	A.insert(2,3,3);
	A.insert(2,4,-5);
	A.insert(3,0,3);
	A.insert(3,1,2);
	A.insert(3,2,3);
	A.insert(3,3,12);
	A.insert(3,4,-1);
	A.insert(4,0,4);
	A.insert(4,1,-3);
	A.insert(4,2,-5);
	A.insert(4,3,-1);
	A.insert(4,4,15);
	

	A.build();

	vector<double> b_used = { 12,-27,14,-17,12 };
	vector<double> x_used;
	x_used.resize(5);
	VectorN<double> b(b_used);
	VectorN<double> x(x_used);



	iterSolver.compute(A, x, b);

	x.forEachIndex([&](size_t i) {
		cout << x[i] << endl;
	});

	//该算例的正确答案： 1.000000 -2.000000  3.000000 -2.000000  1.000000

	return 0;
}