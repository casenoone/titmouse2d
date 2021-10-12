#include <iostream>
using namespace std;

#include "Vector2.h"
int main() {
	
	Vector2<double> v1(1.5, 1.5);
	Vector2<double> v2(2.0, 2.0);
	v2 += v1;
	cout << v2.x << " " << v2.y << endl;
	return 0;
}