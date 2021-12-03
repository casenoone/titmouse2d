#ifndef ARRAY2_HPP
#define ARRAY2_HPP

#include <iostream>
using namespace std;
#include <vector>

#include "Size2.h"

template<class T>
class Array2 {
public:
	Array2();
	~Array2();

	template<typename T>
	using array2Ptr = shared_ptr<Array2<T>>;

	Array2(const vector<vector<T>>& data);

	T lookAt(size_t i,size_t j) const;

	T& operator()(size_t i, size_t j);

	array2Ptr<T> operator+(const Array2<T>& other)const;
	
	array2Ptr<T> operator-(const Array2<T>& other)const;

	template<typename Scalar>
	array2Ptr<T> operator*(const Scalar& other)const;

	Size2 reSize(size_t nx, size_t ny);

	Size2 reSize(size_t nx, size_t ny, T initValue);

	Size2  dataSize() const;

	void set(const Array2<T>& other);

	void swap(Array2<T>& other);

	T infnorm();

	T dot(const Array2<T>& a);

	void increment(T scale, Array2<T>& a);

	void scaleAndIncrement(T scale, Array2<T>& a);

	const vector<vector<T>>* constDataAccessor() const;

	vector<vector<T>>* dataAccessor();

	template<typename Callback>
	void forEachIndex(Callback func) const;


	template <typename Callback>
	void parallelForEachIndex(Callback func);

	//暂时不实现，这个clone只是说克隆出一个新的对象，与当前对象无关
	//T clone();

public:
	vector<vector<T>> _data;

	Size2 _size;
}; 

template<typename T>
using array2Ptr = shared_ptr<Array2<T>>;

template<typename T>
Array2<T>::Array2() {

}

template<typename T>
Array2<T>::~Array2() {

}

template<typename T>
Array2<T>::Array2(const vector<vector<T>>& data) {
	_data = data;
}

template<typename T>
T Array2<T>::lookAt(size_t i, size_t j) const {
	return _data[i][j];
}

template<typename T>
T& Array2<T>::operator()(size_t i, size_t j) {
	return _data[i][j];
}


template<typename T>
array2Ptr<T> Array2<T>::operator+(const Array2<T>& other)const {
	auto ptr = make_shared<Array2<T>>();
	ptr->reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.x; ++j) {
			(*ptr)(i, j) = this->lookAt(i, j) + other.lookAt(i, j);
		}
	}
	return ptr;
}


template<typename T>
array2Ptr<T> Array2<T>::operator-(const Array2<T>& other)const {
	auto ptr = make_shared<Array2<T>>();
	ptr->reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.x; ++j) {
			(*ptr)(i, j) = this->lookAt(i, j) - other.lookAt(i, j);
		}
	}
	return ptr;
}
template<typename T>
template<typename Scalar>
array2Ptr<T> Array2<T>::operator*(const Scalar& other)const {
	auto ptr = make_shared<Array2<T>>();
	ptr->reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.x; ++j) {
			(*ptr)(i, j) = this->lookAt(i, j) * other;
		}
	}
	return ptr;

}


//这里记得并行优化
template<typename T>
Size2 Array2<T>::reSize(size_t nx, size_t ny) {
	_data.resize(nx);
	for (size_t i = 0; i < nx; ++i) {
		_data[i].resize(ny);
	}
	Size2 l(nx, ny);
	_size = l;
	return _size;
}

template<typename T>
Size2 Array2<T>::reSize(size_t nx, size_t ny, T initValue) {
	_data.resize(nx);

	for (size_t i = 0; i < nx; ++i) {
		_data[i].resize(ny);
		for (size_t j = 0; j < ny; ++j) {
			_data[i][j] = initValue;
		}
	}

	
	Size2 l(nx, ny);
	_size = l;
	return _size;
}

template<typename T>
Size2  Array2<T>::dataSize() const{
	return _size;
}

template<typename T>
void Array2<T>::set(const Array2<T>& other) {
	_data = other;
}

template<typename T>
void Array2<T>::swap(Array2<T>& other) {
	std::swap(_data, *(other.dataAccessor()));
}

template<typename T>
T Array2<T>::infnorm() {
	T r = static_cast<T>(0);

	for (size_t i = 0; i < _size.x; ++i) {
		for (size_t j = 0; j < _size.y; ++j) {
			if ((std::fabs(_data[i][j]) > r)) r = std::fabs(_data[i][j]);
		}
	}
	return r;
}

template<typename T>
T Array2<T>::dot(const Array2<T>& a) {
	T r = static_cast<T>(0);

	for (size_t i = 0; i < _size.x; ++i) {
		for (size_t j = 0; j < _size.y; ++j) {
			r += _data[i][j] * a._data[i][j];
		}
	}
	return r;
}

template<typename T>
void Array2<T>::increment(T scale, Array2<T>& a) {
	for (size_t i = 0; i < _size.x; ++i) {
		for (size_t j = 0; j < _size.y; ++j) {
			_data[i][j] += scale * a._data[i][j];
		}
	}
}

template<typename T>
void Array2<T>::scaleAndIncrement(T scale, Array2<T>& a) {
	for (size_t i = 0; i < _size.x; ++i) {
		for (size_t j = 0; j < _size.y; ++j) {
			_data[i][j] = scale * _data[i][j] + a._data[i][j];
		}
	}
}

template<typename T>
const vector<vector<T>>* Array2<T>::constDataAccessor() const {
	return _data;
}

template<typename T>
vector<vector<T>>* Array2<T>::dataAccessor() {
	return _data;
}

template<typename T>
template<typename Callback>
void Array2<T>::forEachIndex(Callback func) const{
	for (size_t j = 0; j < _size.y; ++j) {
		for (size_t i = 0; i < _size.x; ++i) {
			func(i, j);
		}
	}
}


//并行还是先搁置一下吧，不好弄
template<typename T>
template <typename Callback>
void Array2<T>::parallelForEachIndex(Callback func) {

}


#endif