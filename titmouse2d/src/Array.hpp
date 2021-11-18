#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
using namespace std;
#include <vector>

//暂时不实现动态扩容
//不实现arrayAccessor类
//API没有写全，随着项目的完善再增加吧

template<class T>
class Array {
public:
	Array();
	virtual ~Array();

	Array(const vector<T>& data);

	T lookAt(size_t idx) const;

	T& operator[](size_t idx);

	T& operator()(size_t idx);

	size_t reSize(size_t n);

	size_t reSize(size_t n, T initValue);

	size_t dataSize() const;

	const vector<T>* constDataAccessor() const;
	
	vector<T>* dataAccessor() ;

	void clear();

	template<typename Callback>
	void forEachIndex(Callback func) const;


	template <typename Callback>
	void parallelForEachIndex(Callback func) ;



protected:
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
inline void Array<T>::clear() {
	_data.clear();
	_data.resize(_size);
}

template<typename T>
inline T Array<T>::lookAt(size_t idx) const {
	return _data[idx];
}

template<typename T>
inline T& Array<T>::operator[](size_t idx) {
	return _data[idx];
}

template<typename T>
inline T& Array<T>::operator()(size_t idx) {
	return _data[idx];
}

template<typename T>
size_t Array<T>::reSize(size_t n) {
	_size = n;
	_data.resize(n);
	return _size;
}

template<typename T>
size_t Array<T>::reSize(size_t n, T initValue) {
	_size = n;
	_data.assign(n, initValue);
	return _size;
}

template<typename T>
inline size_t Array<T>::dataSize() const{
	return _size;
}

template<typename T>
inline const vector<T>* Array<T>::constDataAccessor() const {
	return &_data;
}


template<typename T>
inline vector<T>* Array<T>::dataAccessor() {
	return &_data;
}



template<typename T>
template<typename Callback>
void Array<T>::forEachIndex(Callback func) const{
	for (size_t i = 0; i < _size; ++i) {
		func(i);
	}
}

template<typename T>
template <typename Callback>
void Array<T>::parallelForEachIndex(Callback func) {
	func(0);
}

#endif