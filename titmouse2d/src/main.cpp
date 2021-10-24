#include <iostream>
using namespace std;

#include "Vector2.hpp"
#include "Array.hpp"
#include "Array2.hpp"

#include "VectorN.hpp"
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
	
	cout << var1 << endl;

	//cout << var1 << endl;


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