#ifndef VECTORGRID2_H
#define VECTORGRID2_H

#include "Grid2.h"
#include "VectorField2.hpp"

template<class T>
class VectorGrid2 :public VectorField2<T>, public Grid2 {
public:
    VectorGrid2();
    virtual ~VectorGrid2();

    void clear();

    void resize(const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin ,
        const T& initialValue);
    
    //��ʱ��ʵ��
    //virtual std::shared_ptr<VectorGrid2> clone() const = 0;

    //��ʱ��ʵ��
    //virtual void fill(const Vector2<double>& value) = 0;

    //ע�⣬�����fillӦ��д��Grid�У�����Ŀǰ�Ҳ����޸��ˣ��õ�ʱ���޸�
    virtual void fill(const T& value);

protected:
    virtual void onResize(const Vector2<size_t>& resolution, const Vector2<double>& gridSpacing,
        const Vector2<double>& origin,
        const T& initialValue) = 0;
};




template<class T>
VectorGrid2<T>::VectorGrid2() {

}


template<class T>
VectorGrid2<T>::~VectorGrid2() {

}

template<class T>
void VectorGrid2<T>::clear() {
    resize(Vector2<size_t>(), gridSpacing(), origin(), Vector2<double>());
}

template<class T>
void VectorGrid2<T>::resize(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    const T& initialValue) {

    //����һ����˵

    //setSizeParameters(resolution, gridSpacing, origin);

    _resolution = resolution;
    _gridSpacing = gridSpacing;
    _origin = origin;

    onResize(resolution, gridSpacing, origin, initialValue);
}


template<class T>
void VectorGrid2<T>::fill(const T& value) {

}





#endif