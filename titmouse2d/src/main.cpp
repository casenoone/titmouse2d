﻿#include <iostream>
using namespace std;

#include "Vector2.hpp"
#include "Array.hpp"
#include "Array2.hpp"
#include "VectorN.hpp"
#include "DenseMatrix.hpp"

#include <array>

int main() {
	
	


	vector<int> temp1, temp2;

	for (int i = 1; i < 4; ++i) {
		temp1.push_back(i);
	}

	for (int i = 1; i < 4; ++i) {
		temp2.push_back(i);
	}

	VectorX<int> vec1(temp1);
	VectorY<int> vec2(temp2);

	auto var1 = vec1 * vec2;
	

	vector<vector<int>> tempMat1 = {
		{1,2,7},
		{2,0,1},
		{0,1,1}
	};

	vector<vector<int>> tempMat2 = {
		{1,1,0},
		{3,1,2},
		{1,4,0}
	};

	DenseMatrix<int> mat1(3, 3, tempMat1);
	DenseMatrix<int> mat2(3, 3, tempMat2);

	auto mat3 = mat1 * mat2;
	mat3->forEachIndex([&](size_t i,size_t j){
		
		//cout << mat3->lookAt(i, j) << endl;
			
	});

	auto mat4 = 5 * mat1;

	mat4->setOrder(ROW);

	mat1.forEachIndex([&](size_t i, size_t j) {

		cout << mat1.lookAt(i, j) << endl;

	});

	
	
	auto kk = mat4->getVec(0);
	
	//mat4->setOrder(COLUMN);
	//VectorY<int> temp1;

	//shared_ptr<VectorX<int>> p1 = make_shared<VectorX<int>>();

	//shared_ptr<VectorY<int>> p2 = make_shared<VectorY<int>>();

	////派生类转为基类
	//p1 = dynamic_pointer_cast<VectorX<int>>(p2);

	//cout << typeid(p1).name() << endl;

	//p2 = dynamic_pointer_cast<VectorY<int>>(p1);

	//cout << typeid(p2).name() << endl;

	return 0;
}