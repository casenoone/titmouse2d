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
	
	
	vector<int> tempVec = { 1,0,2,3 };

	VectorN<int> vec(tempVec);

	vec.forEachIndex([&](size_t i) {
		
		//cout << vec(i) << endl;
	});

	SparseMatrix<int> testMat(4, 3);
	testMat.insert(0,0,1);
	testMat.insert(0,2,8);
	testMat.insert(1,0,2);
	testMat.insert(2,0,1);
	testMat.insert(2,1,2);
	testMat.insert(3,0,1);

	testMat.build();

	SparseMatrix<int> testMat1(3, 2);
	testMat1.insert(0,0,1);
	testMat1.insert(0,1,6);
	testMat1.insert(1,1,1);
	testMat1.insert(2,0,4);
	testMat1.insert(2, 1, 2);
	testMat1.build();

	testMat1.forEachIndex([&](size_t i, size_t j) {
		//cout << testMat1.lookAt(i, j) << endl;
	});

	auto vecN = vec * testMat;
	*vecN = *((*vecN) + (*vecN));
	vecN->forEachIndex([&](size_t i) {
		cout << vecN->lookAt(i);
	});

	return 0;
}