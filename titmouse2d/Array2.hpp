#ifndef ARRAY2_H
#define ARRAY2_H

#include <iostream>
using namespace std;
#include <vector>

#include "Size2.h"

template<class T>
class Array2 {
public:
	Array2();
	~Array2();

	Array2(const vector<vector<T>>& data);

	T lookAt(size_t i,size_t j) const;

	T& operator()(size_t i, size_t j);

	Size2 reSize(size_t nx, size_t ny);

	Size2 reSize(size_t nx, size_t ny, T initValue);

	Size2  dataSize();

	const vector<vector<T>>* constDataAccessor() const;

	vector<vector<T>>* dataAccessor();

private:
	vector<vector<T>> _data;

	Size2 _size;
}; 

template<typename T>
using array2Ptr = shared_ptr<Array2<T>>;

#endif