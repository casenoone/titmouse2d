#ifndef ARRAY2_HPP
#define ARRAY2_HPP

#include <vector>

#include "Size2.h"


template<class T>
class Array2 {
public:
	Array2();

	Array2(const std::vector<std::vector<T>>& data);

	T lookAt(int i, int j) const;

	T& operator()(int i, int j);


	void shallowCopy(Array2<T>& other);

	//实现智能指针类的深拷贝
	//实际上添加一个copy方法更好
	Array2<T>& operator=(const Array2<T>& other);

	Array2<T> operator+(const Array2<T>& other)const;

	Array2<T> operator-(const Array2<T>& other)const;

	template<typename Scalar>
	Array2<T> operator*(const Scalar& other)const;

	Size2 reSize(int nx, int ny);

	Size2 reSize(int nx, int ny, T initValue);

	Size2  dataSize() const;

	void set(const Array2<T>& other);

	void swap(Array2<T>& other);

	T dot(const Array2<T>& a);

	template<typename Callback>
	void forEachIndex(Callback func) const;

	//暂时不实现，这个clone只是说克隆出一个新的对象，与当前对象无关
	//T clone();

public:
	//这里换成一维的std::vector
	std::shared_ptr<std::vector<std::vector<T>>> _data;

	Size2 _size;
};

using Array2I = Array2<int>;
using Array2F = Array2<float>;
using Array2D = Array2<double>;

template<typename T>
Array2<T>::Array2() {
	_data = std::make_shared<std::vector<std::vector<T>>>();
}


template<typename T>
Array2<T>::Array2(const std::vector<std::vector<T>>& data) {
	_data = std::make_shared<std::vector<std::vector<T>>>();
	(*_data) = data;
}

template<typename T>
T Array2<T>::lookAt(int i, int j) const {
	return (*_data)[i][j];
}


template<typename T>
T& Array2<T>::operator()(int i, int j) {
	return (*_data)[i][j];
}

template<typename T>
void Array2<T>::shallowCopy(Array2<T>& other) {
	_size = other._size;
	_data = other._data;
}

template<typename T>
Array2<T>& Array2<T>::operator=(const Array2<T>& other) {
	(*_data) = (*(other._data));
	_size = other._size;
	return *this;
}


template<typename T>
Array2<T> Array2<T>::operator+(const Array2<T>& other)const {
	Array2<T> result;
	result.reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.y; ++j) {
			(*(result._data))(i, j) = this->lookAt(i, j) + other.lookAt(i, j);
		}
	}
	return result;
}


template<typename T>
Array2<T> Array2<T>::operator-(const Array2<T>& other)const {
	Array2<T> result;
	result.reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.y; ++j) {
			(*(result._data))(i, j) = this->lookAt(i, j) - other.lookAt(i, j);
		}
	}
	return result;
}


template<typename T>
template<typename Scalar>
Array2<T> Array2<T>::operator*(const Scalar& other)const {
	Array2<T> result;
	result.reSize(_size.x, _size.y);
	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.y; ++j) {
			(*(result._data))(i, j) = this->lookAt(i, j) * other;
		}
	}
	return result;

}


//这里记得并行优化
template<typename T>
Size2 Array2<T>::reSize(int nx, int ny) {
	(*_data).resize(nx);
	for (int i = 0; i < nx; ++i) {
		(*_data)[i].resize(ny);
	}
	Size2 l(nx, ny);
	_size = l;
	return _size;
}

template<typename T>
Size2 Array2<T>::reSize(int nx, int ny, T initValue) {
	(*_data).resize(nx);

	for (int i = 0; i < nx; ++i) {
		(*_data)[i].resize(ny);
		for (int j = 0; j < ny; ++j) {
			(*_data)[i][j] = initValue;
		}
	}


	Size2 l(nx, ny);
	_size = l;
	return _size;
}



template<typename T>
Size2  Array2<T>::dataSize() const {
	return _size;
}

template<typename T>
void Array2<T>::set(const Array2<T>& other) {
	(*_data) = *(other._data);
	_size = other._size;
}

template<typename T>
void Array2<T>::swap(Array2<T>& other) {
	std::swap((*_data), (*(other._data)));
}


template<typename T>
T Array2<T>::dot(const Array2<T>& a) {
	T r = static_cast<T>(0);

	for (int i = 0; i < _size.x; ++i) {
		for (int j = 0; j < _size.y; ++j) {
			r += (*_data)[i][j] * (*(a._data)[i][j]);
		}
	}
	return r;
}






template<typename T>
template<typename Callback>
void Array2<T>::forEachIndex(Callback func) const {
	for (int j = 0; j < _size.y; ++j) {
		for (int i = 0; i < _size.x; ++i) {
			func(i, j);
		}
	}
}




#endif