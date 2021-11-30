#ifndef CELLCENTEREDVECTORGRID2_H
#define CELLCENTEREDVECTORGRID2_H


#include "CollocatedVectorGrid2.hpp"

template<class T>
class CellCenteredVectorGrid2 : public CollocatedVectorGrid2<T>{
public:
	
	//������ʱ��ʵ��Builder��
	//class Builder;

    /*template<class T>
    using CellCenteredVectorGrid2Ptr = shared_ptr<CellCenteredVectorGrid2<T>>;*/

    CellCenteredVectorGrid2();

    virtual ~CellCenteredVectorGrid2();

    CellCenteredVectorGrid2(
        const Vector2<size_t>& resolution,
        const Vector2<double>& origin,
        const T& initialValue);

    CellCenteredVectorGrid2(const CellCenteredVectorGrid2<T>& other);

    Vector2<size_t> dataSize() const override;

    Vector2<double> dataOrigin() const override;

    //��ʱ��ʵ��
    //void swap(Grid2* other) override;

    void set(const CellCenteredVectorGrid2<T>& other);

    CellCenteredVectorGrid2<T>& operator=(const CellCenteredVectorGrid2<T>& other);

    //��ʱ��ʵ��
    //std::shared_ptr<VectorGrid2> clone() const override;

    //��ʱ��ʵ��
    //static Builder builder();

};

template<class T>
using CellCenteredVectorGrid2Ptr = shared_ptr<CellCenteredVectorGrid2<T>>;

template<class T>
CellCenteredVectorGrid2<T>:: CellCenteredVectorGrid2() {
    cout << 777 << endl;
}

template<class T>
CellCenteredVectorGrid2<T>::~CellCenteredVectorGrid2() {

}

template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2
(
    const Vector2<size_t>& resolution,
    const Vector2<double>& origin,
    const T& initialValue) {
   
    auto newgridspacing = Vector2<double>(2.0 / resolution.x, 2.0 / resolution.y);
    this->resize(resolution, newgridspacing, origin, initialValue);

    this->_linearSampler->_accessor = this->_data;
 
}


template<class T>
CellCenteredVectorGrid2<T>::CellCenteredVectorGrid2(const CellCenteredVectorGrid2& other) {

}


template<class T>
Vector2<size_t> CellCenteredVectorGrid2<T>::dataSize() const {
    return this->resolution();
}


template<class T>
Vector2<double> CellCenteredVectorGrid2<T>::dataOrigin() const {
    return this->origin() + this->gridSpacing() * 0.5;
}


//��ʱ��ʵ�����ڲ�
//template<class T>
//void CellCenteredVectorGrid2<T>::swap(Grid2* other) {
//
//}


template<class T>
void CellCenteredVectorGrid2<T>::set(const CellCenteredVectorGrid2<T>& other) {

}


//�ݲ�ʵ�����ڲ�
template<class T>
CellCenteredVectorGrid2<T>& CellCenteredVectorGrid2<T>::operator=(const CellCenteredVectorGrid2<T>& other) {

}



#endif