#include <iostream>
using namespace std;

#include "Vector2.hpp"
#include "Array.hpp"
#include "Array2.hpp"

#include "VectorN.hpp"
int main() {
	
	Vector2<double> v1(1.5, 1.5);
	Vector2<double> v2(2.0, 2.0);
	v2 += v1;
	2.0 * v1;

	const double pi = 2;
	const Vector2<double> m(1.0, 1.0);

	2.0 * m;

	cout << (2.0 * m).x << endl;

	vector<double> x;
	for (int i = 0; i < 10; ++i) {
		x.push_back(i);
	}

	arrayPtr<double> p = make_shared < Array < double >> (x);
	
	p->reSize(20, 10);


	p->forEachIndex([&](size_t i) {
		(*p)[i] = 0;

	});

	VectorX<int> temp;

	VectorY<int> temp1;

	shared_ptr<VectorX<int>> p1 = make_shared<VectorX<int>>();

	shared_ptr<VectorY<int>> p2 = make_shared<VectorY<int>>();

	//派生类转为基类
	p1 = dynamic_pointer_cast<VectorX<int>>(p2);

	cout << typeid(p1).name() << endl;

	p2 = dynamic_pointer_cast<VectorY<int>>(p1);

	cout << typeid(p2).name() << endl;

	return 0;
}