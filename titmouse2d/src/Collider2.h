#ifndef COLLIDER2_H
#define COLLIDER2_H

#include "Geometry/Surface2.h"

#include <iostream>
using namespace std;
#include<vector>

//ע������collider���÷�
//Ϊ��ʵ�ֶ�̬
//ֻ������pushָ�����͵�surface

class Collider2 {
public:
	Collider2();

	~Collider2();

	void push(const Surface2Ptr& _surface);

public:
	vector<Surface2Ptr> _surfaces;
};


#endif