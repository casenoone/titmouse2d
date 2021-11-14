#ifndef ARRAYSAMPLER2_HPP
#define ARRAYSAMPLER2_HPP

#include "../Vector2.hpp"
#include <functional>

#include "../Array2Ptr.hpp"
#include "../MathUtils.hpp"

#include <iostream>

//线性采样器

template<class T>
class LinearArraySampler2 {
public:

	LinearArraySampler2();

    ~LinearArraySampler2();

    LinearArraySampler2(
        const Array2Ptr<T>& accessor,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& gridOrigin);

    LinearArraySampler2(const LinearArraySampler2& other);

    double operator()(const Array2Ptr<T>& accessor, const Vector2<double>& x);

    void getCoordinatesAndWeights(
        const Vector2<double>& x,
        std::array<Vector2<int>, 4>* indices,
        std::array<double, 4>* weights);


public:
    Vector2<double> _gridSpacing;
    Vector2<double> _invGridSpacing;
    Vector2<double> _origin;
    Array2Ptr<T> _accessor;
};

template<typename T>
using LinearArraySampler2Ptr = shared_ptr<LinearArraySampler2<T>>;




template<class T>
LinearArraySampler2<T>::LinearArraySampler2() {

}

template<class T>
LinearArraySampler2<T>::~LinearArraySampler2() {

}



template<class T>
LinearArraySampler2<T>::LinearArraySampler2(
    const Array2Ptr<T>& accessor,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& gridOrigin) {

    _gridSpacing = gridSpacing;
    _invGridSpacing = Vector2<double>(1 / gridSpacing.x, 1 / gridSpacing.y);
    _origin = gridOrigin;
    _accessor = accessor;

}

template<class T>
LinearArraySampler2<T>::LinearArraySampler2(const LinearArraySampler2<T>& other) {

    _gridSpacing = other._gridSpacing;
    _invGridSpacing = other._invGridSpacing;
    _origin = other._origin;
    _accessor = other._accessor;

}


template<class T>
double LinearArraySampler2<T>::operator()(const Array2Ptr<T>& accessor, const Vector2<double>& x) {
    int i, j;
    double fx, fy;
    Vector2<double> normalizedX = (x - _origin) / _gridSpacing;
    auto& accessors = const_cast<Array2Ptr<double>&>(accessor);
    auto size = accessors.dataSize();
    int iSize = size.x;
    int jSize = size.y;
    getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
    int ip1 = std::min(i + 1, iSize - 1);
    int jp1 = std::min(j + 1, jSize - 1);


    ////双线性插值
    return bilerp(
        accessors(i, j),
        accessors(ip1, j),
        accessors(i, jp1),
        accessors(ip1, jp1),
        fx,
        fy);

}


//计算粒子在各个节点上的插值
template<class T>
void LinearArraySampler2<T>::getCoordinatesAndWeights(
    const Vector2<double>& x,
    std::array<Vector2<int>, 4>* indices,
    std::array<double, 4>* weights) {

    //这步操作在做这样一个事情：
    //把粒子坐标映射到某个网格单元的局部坐标，然后将新坐标单位化
    //为的是保证权重之和为1

    
    //这里的origin指的是背景网格的起点，而非每一个小cell里的起点
    Vector2<double> normalizedX = (x - _origin) / _gridSpacing;

    auto size = _accessor.dataSize();
    int iSize = size.x;
    int jSize = size.y;

    int i, j;
    double fx, fy;

    //得到粒子所在的格子的下标以及粒子在格子中的相对坐标
    getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    //i，j实际上就是粒子所在的格子的索引
    //要求一个cell中四个节点的索引
    int ip1 = std::min(i + 1, iSize - 1);
    int jp1 = std::min(j + 1, jSize - 1);

    (*indices)[0] = Vector2<int>(i, j);
    (*indices)[1] = Vector2<int>(ip1, j);
    (*indices)[2] = Vector2<int>(i, jp1);
    (*indices)[3] = Vector2<int>(ip1, jp1);

    //双线性插值
    (*weights)[0] = (1 - fx) * (1 - fy);
    (*weights)[1] = fx * (1 - fy);
    (*weights)[2] = (1 - fx) * fy;
    (*weights)[3] = fx * fy;

}


#endif