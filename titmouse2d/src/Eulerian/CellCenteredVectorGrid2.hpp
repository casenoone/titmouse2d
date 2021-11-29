#ifndef CELLCENTEREDVECTORGRID2_H
#define CELLCENTEREDVECTORGRID2_H


#include "CollocatedVectorGrid2.hpp"

template<class T>
class CellCenteredVectorGrid2 : public CollocatedVectorGrid2<T>{
public:
	
	//这里暂时不实现Builder类
	//class Builder;

    template<class T>
    using CellCenteredVectorGrid2Ptr = shared_ptr<CellCenteredVectorGrid2<T>>;

    CellCenteredVectorGrid2();

    CellCenteredVectorGrid2(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin,
        const T& initialValue);

    CellCenteredVectorGrid2(const CellCenteredVectorGrid2<T>& other);

    Vector2<size_t> dataSize() const override;

    Vector2<double> dataOrigin() const override;

    void swap(Grid2* other) override;

    void set(const CellCenteredVectorGrid2<T>& other);

    CellCenteredVectorGrid2<T>& operator=(const CellCenteredVectorGrid2<T>& other);

    //暂时不实现
    //std::shared_ptr<VectorGrid2> clone() const override;

    //暂时不实现
    //static Builder builder();

};

template<class T>
using CellCenteredVectorGrid2Ptr = shared_ptr<CellCenteredVectorGrid2<T>>;

template<class T>
CellCenteredVectorGrid2<T>:: CellCenteredVectorGrid2() {

}


template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    const T& initialValue) {

}


template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2(const CellCenteredVectorGrid2& other) {

}


template<class T>
Vector2<size_t> CellCenteredVectorGrid2<T>::dataSize() const {

}


template<class T>
Vector2<double> CellCenteredVectorGrid2<T>::dataOrigin() const {

}


template<class T>
void CellCenteredVectorGrid2<T>::swap(Grid2* other) {

}


template<class T>
void CellCenteredVectorGrid2<T>::set(const CellCenteredVectorGrid2<T>& other) {

}


template<class T>
CellCenteredVectorGrid2<T>& CellCenteredVectorGrid2<T>::operator=(const CellCenteredVectorGrid2<T>& other) {

}



#endif