#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;
#include <vector>

//û����ͨ����ArrayAccessor�ı�Ҫ��

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