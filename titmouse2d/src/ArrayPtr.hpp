#ifndef ARRAYPTR_HPP
#define ARRAYPTR_HPP

#include "Array.hpp"


template<class T>
class ArrayPtr {
public:
	ArrayPtr();

	virtual ~ArrayPtr();

	ArrayPtr(const vector<T>& data);

	T lookAt(size_t idx) const;

	T& operator[](size_t idx);

	T& operator()(size_t idx);

	//±‹√‚…ÓøΩ±¥
	void operator=(const ArrayPtr<T>& arrPtr);

	size_t& reSize(size_t n);

	size_t& reSize(size_t n, T initValue);

	size_t dataSize() const;

	void clear();

	template<typename Callback>
	void forEachIndex(Callback func) const;

protected:
	shared_ptr<Array<T>> arrayPtr;
};


template<typename T>
ArrayPtr<T>::ArrayPtr() {
	arrayPtr = make_shared<Array<T>>(vector<T>());
}

template<typename T>
ArrayPtr<T>::~ArrayPtr() {

}

template<typename T>
ArrayPtr<T>::ArrayPtr(const vector<T>& data) {
	arrayPtr = make_shared<Array<T>>(data);
}

template<typename T>
void ArrayPtr<T>::clear() {
	this->arrayPtr->clear();
}

template<typename T>
inline T ArrayPtr<T>::lookAt(size_t idx) const {
	return arrayPtr->lookAt(idx);
}

template<typename T>
inline T& ArrayPtr<T>::operator[](size_t idx) {
	return (*arrayPtr)[idx];
}

template<typename T>
inline T& ArrayPtr<T>::operator()(size_t idx) {
	return (*arrayPtr)[idx];
}

template<typename T>
inline void ArrayPtr<T>::operator=(const ArrayPtr<T>& arrPtr) {
	*(this->arrayPtr) = *(arrPtr.arrayPtr);
}



template<typename T>
size_t& ArrayPtr<T>::reSize(size_t n) {
	arrayPtr->reSize(n);
	return n;
}

template<typename T>
size_t& ArrayPtr<T>::reSize(size_t n, T initValue) {
	arrayPtr->reSize(n,initValue);
	return n;
}

template<typename T>
inline size_t ArrayPtr<T>::dataSize() const {
	return arrayPtr->dataSize();
}



template<typename T>
template<typename Callback>
void ArrayPtr<T>::forEachIndex(Callback func) const {
	for (size_t i = 0; i < arrayPtr->dataSize(); ++i) {
		func(i);
	}
}





#endif