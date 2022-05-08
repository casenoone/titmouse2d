#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <vector>

//不实现arrayAccessor类
//API没有写全，随着项目的完善再增加吧

//改成由共享指针管理的类
//实现浅拷贝

template<class T>
class Array {
public:
	Array();

	virtual ~Array();

	Array(const std::vector<T>& data);

	Array(int len);

	T lookAt(int idx) const;

	T& operator[](int idx);

	T& operator()(int idx);

	int reSize(int n);

	int reSize(int n, T initValue);

	int dataSize() const;

	//从尾处添加元素
	void push(const T& e);


	void set(const std::vector<T>& data_);

	void clear();

	template<typename Callback>
	void forEachIndex(Callback func) const;


	void pop_back() {
		_data->pop_back();
		_size--;
	}

protected:
	std::shared_ptr<std::vector<T>> _data;

	int _size;
};

using ArrayI = Array<int>;
using ArrayF = Array<float>;
using ArrayD = Array<double>;


template<typename T>
Array<T>::Array() {
	_data = std::make_shared<std::vector<T>>();
	_size = 0;
}

template<typename T>
Array<T>::~Array() {

}


template<typename T>
Array<T>::Array(const std::vector<T>& data) {

	_data = std::make_shared<std::vector<T>>();

	(*_data) = data;
	_size = (*_data).size();
}

template<typename T>
Array<T>::Array(int len) {
	_data = std::make_shared<std::vector<T>>();
	(*_data).resize(len);
	_size = len;
}

//这里要改进一下
template<typename T>
inline void Array<T>::clear() {
	(*_data).clear();
	(*_data).resize(_size);
}

template<typename T>
inline T Array<T>::lookAt(int idx) const {
	return (*_data)[idx];
}

template<typename T>
inline T& Array<T>::operator[](int idx) {
	return (*_data)[idx];
}

template<typename T>
inline T& Array<T>::operator()(int idx) {
	return (*_data)[idx];
}

template<typename T>
int Array<T>::reSize(int n) {
	_size = n;
	(*_data).resize(n);
	return _size;
}

template<typename T>
int Array<T>::reSize(int n, T initValue) {
	_size = n;
	(*_data).assign(n, initValue);
	return _size;
}

template<typename T>
int Array<T>::dataSize() const {
	return _size;
}

template<typename T>
void Array<T>::push(const T& e) {
	++_size;
	(*_data).push_back(e);
}



template<typename T>
void Array<T>::set(const std::vector<T>& data_) {
	(*_data) = data_;
	_size = (*_data).size();
}


template<typename T>
template<typename Callback>
void Array<T>::forEachIndex(Callback func) const {
	for (int i = 0; i < _size; ++i) {
		func(i);
	}
}


#endif