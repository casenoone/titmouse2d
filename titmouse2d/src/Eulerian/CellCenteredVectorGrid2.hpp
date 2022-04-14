#ifndef CELLCENTEREDVECTORGRID2_H
#define CELLCENTEREDVECTORGRID2_H


#include "CollocatedVectorGrid2.hpp"

template<class T>
class CellCenteredVectorGrid2 : public CollocatedVectorGrid2<T> {
public:

	//这里暂时不实现Builder类
	//class Builder;

	/*template<class T>
	using CellCenteredVectorGrid2Ptr = shared_ptr<CellCenteredVectorGrid2<T>>;*/

	CellCenteredVectorGrid2();

	virtual ~CellCenteredVectorGrid2();

	CellCenteredVectorGrid2(
		const Vector2I& resolution,
		const Vector2D& origin,
		const T& initialValue);

	CellCenteredVectorGrid2(const CellCenteredVectorGrid2<T>& other);

	Vector2I dataSize() const override;

	Vector2D dataOrigin() const override;

	//暂时不实现
	//void swap(Grid2* other) override;

	void set(const CellCenteredVectorGrid2<T>& other);

	CellCenteredVectorGrid2<T>& operator=(const CellCenteredVectorGrid2<T>& other);

	//暂时不实现
	//std::shared_ptr<VectorGrid2> clone() const override;
	std::shared_ptr<CellCenteredVectorGrid2> clone() const;


	//暂时不实现
	//static Builder builder();

};

template<class T>
using CellCenteredVectorGrid2Ptr = std::shared_ptr<CellCenteredVectorGrid2<T>>;

template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2() {
}

template<class T>
CellCenteredVectorGrid2<T>::~CellCenteredVectorGrid2() {

}

template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2
(
	const Vector2I& resolution,
	const Vector2D& origin,
	const T& initialValue) {

	auto newgridspacing = Vector2D(2.0 / resolution.x, 2.0 / resolution.y);
	this->resize(resolution, newgridspacing, origin, initialValue);
	this->_linearSampler = std::make_shared<LinearArraySampler2<T>>(this->_data, newgridspacing, dataOrigin());

}


template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2(const CellCenteredVectorGrid2& other) {
	(*this) = other;
}


template<class T>
Vector2I CellCenteredVectorGrid2<T>::dataSize() const {
	return this->resolution();
}


template<class T>
Vector2D CellCenteredVectorGrid2<T>::dataOrigin() const {
	return this->origin() + this->gridSpacing() * 0.5;

}


//暂时不实现其内部
//template<class T>
//void CellCenteredVectorGrid2<T>::swap(Grid2* other) {
//
//}


template<class T>
void CellCenteredVectorGrid2<T>::set(const CellCenteredVectorGrid2<T>& other) {

}


//这样做可能会有隐患
template<class T>
CellCenteredVectorGrid2<T>& CellCenteredVectorGrid2<T>::operator=(const CellCenteredVectorGrid2<T>& other) {
	this->_data = other->data;
	*(this->_linearSampler) = *(other._linearSampler);
}

template<class T>
std::shared_ptr<CellCenteredVectorGrid2<T>> CellCenteredVectorGrid2<T>::clone() const {
	CellCenteredVectorGrid2 newGrid;
	newGrid = (*this);
	auto result = std::make_shared<CellCenteredVectorGrid2<T>>(newGrid);
	return result;
}




#endif