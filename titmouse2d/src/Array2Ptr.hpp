#ifndef ARRAYPTR2_HPP
#define ARRAYPTR2_HPP

#include "Array2.hpp"

template<class T>
class Array2Ptr {
public:
	Array2Ptr();
	~Array2Ptr();

	Array2Ptr(const vector<vector<T>>& data);

	T lookAt(size_t i, size_t j) const;

	T& operator()(size_t i, size_t j);

	void operator=(const Array2Ptr<T>& obj);

	Size2 reSize(size_t nx, size_t ny);

	Size2 reSize(size_t nx, size_t ny, T initValue);

	Size2  dataSize() const;

	void set(const Array2Ptr<T>& other);

	vector<vector<T>>* dataAccessor();

	void swap(Array2Ptr<T>& other);

	T infnorm();

	T dot(const Array2Ptr<T>& a);

	void increment(T scale, Array2Ptr<T>& a);

	void scaleAndIncrement(T scale, Array2Ptr<T>& a);

	template<typename Callback>
	void forEachIndex(Callback func) const;

private:
	shared_ptr<Array2<T>> array2Ptr;
	
};


template<typename T>
Array2Ptr<T>::Array2Ptr() {
	array2Ptr = make_shared<Array2<T>>(vector<vector<T>>());
}

template<typename T>
Array2Ptr<T>::~Array2Ptr() {
	
}

template<typename T>
Array2Ptr<T>::Array2Ptr(const vector<vector<T>>& data) {
	array2Ptr = make_shared<Array2<T>>(data);
}

template<typename T>
T Array2Ptr<T>::lookAt(size_t i, size_t j) const {
	return array2Ptr->lookAt(i, j);
}

template<typename T>
T& Array2Ptr<T>::operator()(size_t i, size_t j) {
	return (*array2Ptr)(i,j);
}




template<typename T>
inline void Array2Ptr<T>::operator=(const Array2Ptr<T>& obj) {
	*(this->array2Ptr) = *(obj.array2Ptr);
}

//这里记得并行优化
template<typename T>
Size2 Array2Ptr<T>::reSize(size_t nx, size_t ny) {
	return array2Ptr->reSize(nx, ny);
}

template<typename T>
Size2 Array2Ptr<T>::reSize(size_t nx, size_t ny, T initValue) {
	
	return array2Ptr->reSize(nx, ny, initValue);
}

template<typename T>
Size2  Array2Ptr<T>::dataSize() const {
	return array2Ptr->dataSize();
}

template<typename T>
void Array2Ptr<T>::set(const Array2Ptr<T>& other) {
	array2Ptr->set(*(other->array2Ptr));
}


template<typename T>
vector<vector<T>>* Array2Ptr<T>::dataAccessor() {
	return array2Ptr->dataAccessor();
}

template<typename T>
void Array2Ptr<T>::swap(Array2Ptr<T>& other) {
	array2Ptr->swap(*(other->array2Ptr));
}

template<typename T>
T Array2Ptr<T>::infnorm() {
	return array2Ptr->infnorm();
}

template<typename T>
T Array2Ptr<T>::dot(const Array2Ptr<T>& a) {
	return array2Ptr->dot(*(a->array2Ptr));
}

template<typename T>
void Array2Ptr<T>::increment(T scale, Array2Ptr<T>& a) {
	array2Ptr->increment(scale, *(a->array2Ptr));
}

template<typename T>
void Array2Ptr<T>::scaleAndIncrement(T scale, Array2Ptr<T>& a) {
	array2Ptr->scaleAndIncrement(scale, *(a->array2Ptr));
}



template<typename T>
template<typename Callback>
void Array2Ptr<T>::forEachIndex(Callback func) const {
	auto _size = array2Ptr->dataSize();
	for (size_t j = 0; j < _size.y; ++j) {
		for (size_t i = 0; i < _size.x; ++i) {
			func(i, j);
		}
	}
}



#endif