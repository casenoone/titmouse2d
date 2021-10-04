#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;
#include <vector>

//没有想通设置ArrayAccessor的必要性

template<class T>
class Array {
public:
	Array();
	~Array();



private:
	vector<T> _data;

	size_t _size;
};

#endif