#include <iostream>
using namespace std;

#include "Vector2.hpp"
#include "Array.hpp"
#include "Array2.hpp"
int main() {
	
	Vector2<double> v1(1.5, 1.5);
	Vector2<double> v2(2.0, 2.0);
	v2 += v1;
	cout << v2.x << " " << v2.y << endl;

	vector<double> x;
	for (int i = 0; i < 10; ++i) {
		x.push_back(i);
	}

	arrayPtr<double> p = make_shared < Array < double >> (x);
	
	auto k = p->dataAccessor();
	auto k1 = p->constDataAccessor();
	(*k)[5] = 0;

	p->reSize(7, 10);

	cout << (*k)[5]<<endl;

	return 0;
}