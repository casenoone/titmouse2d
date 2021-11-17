#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"

#include <iostream>
using namespace std;
#include<vector>

//注意这里collider的用法
//为了实现多态
//只能往里push指针类型的surface

class Collider2 {
public:
	Collider2();

	~Collider2();

	void push(const Surface2Ptr& _surface);

public:
	vector<Surface2Ptr> _surfaces;
};


#endif