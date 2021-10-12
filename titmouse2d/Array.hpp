#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
using namespace std;
#include <vector>

//暂时不实现动态扩容


template<class T>
class Array {
public:
	Array();
	~Array();

	Array(const vector<T>& data);

	T lookAt(size_t idx) const;

	T& operator[](size_t idx);

	size_t& reSize(size_t n);

	size_t& reSize(size_t n, T initValue);

	size_t dataSize();

	const vector<T>* constDataAccessor() const;
	
	vector<T>* dataAccessor() ;

private:
	vector<T> _data;

	size_t _size;
};

template<typename T>
using arrayPtr = shared_ptr<Array<T>>;


template<typename T>
Array<T>::Array() {

}

template<typename T>
Array<T>::~Array() {

}

template<typename T>
Array<T>::Array(const vector<T>& data) {
	_data = data;
	_size = _data.size();
}

template<typename T>
T Array<T>::lookAt(size_t idx) const {
	return _data[idx];
}

template<typename T>
T& Array<T>::operator[](size_t idx) {
	return _data[idx];
}

template<typename T>
size_t& Array<T>::reSize(size_t n) {
	_data.resize(n);
	return n;
}

template<typename T>
size_t& Array<T>::reSize(size_t n, T initValue) {
	_data.assign(n, initValue);
	return n;
}

template<typename T>
size_t Array<T>::dataSize() {
	return _size;
}

template<typename T>
const vector<T>* Array<T>::constDataAccessor() const {
	return &_data;
}


template<typename T>
vector<T>* Array<T>::dataAccessor() {
	return &_data;
}


#endif