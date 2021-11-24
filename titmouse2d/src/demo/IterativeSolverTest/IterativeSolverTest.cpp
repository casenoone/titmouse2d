//#include <iostream>
//using namespace std;
//
//
//#include "../../../src/LinearSystem/SteepestDescentSolver.hpp"
//#include "../../../src/LinearSystem/ConjugateGradientSolver.hpp"
//#include "../../../src/LinearSystem/JacobiSolver.hpp"
//#include "../../../src/LinearSystem/GaussSeidelSolver.hpp"
//
//#include <array>
//
//int main() {
//
//
//	SteepestDescentSolver<double> SteepSolver;
//
//	ConjugateGradientSolver <double> cgSolver;
//
//	JacobiSolver <double> jSolver;
//
//	GaussSeidelSolver <double> gaussSolver;
//
//	vector<vector<double>> A_data = {
//		{10.0, 1.0, 2.0,3.0,4.0},
//		{1.0, 9.0,-1.0,2.0,-3.0},
//		{2.0,-1.0,7.0,3.0,-5},
//		{3.0,2.0,3.0,12.0,-1.0},
//		{4.0,-3.0,-5.0,-1.0,15.0}
//	};
//
//	vector<vector<
//	double>> A_data = {
//		{4.,-1.,0.},
//		{-1.,4.,-1.},
//		{0.,-1.,4.}
//	};
//
//	SparseMatrixPtr<double> A(A_data);
//
//	
//
//	vector<double> b_used = { 12,-27,14,-17,12 };
//	vector<double> b_used = { 1.,4.,-3. };
//	vector<double> x_used;
//	x_used.resize(5);
//	VectorNPtr<double> b(b_used);
//	VectorNPtr<double> x(x_used);
//
//	b *= -1;
//	b.forEachIndex([&](size_t i) {
//		cout << b[i] << endl;
//		});
//
//
//
//	SteepSolver.compute(A, x, b);
//	cgSolver.compute(A, x, b);
//	jSolver.compute(A, x, b);
//	gaussSolver.compute(A, x, b);
//	x.forEachIndex([&](size_t i) {
//		cout << x[i] << endl;
//		});
//
//	该算例的正确答案： 1.000000 -2.000000  3.000000 -2.000000  1.000000
//
//
//	该算例的正确答案： 0.499999 1  -0.49999 
//
//
//
//
//
//	return 0;
//}