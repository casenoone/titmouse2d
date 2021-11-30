#ifndef COLLOCATEDVECTORGRID2_HPP
#define COLLOCATEDVECTORGRID2_HPP


#include "VectorGrid2.hpp"
#include "../Array2Ptr.hpp"
#include "ArraySampler2.hpp"
#include <iostream>
using namespace std;

#include <vector>

template<class T>
class CollocatedVectorGrid2 : public VectorGrid2<T> {
public:
    

    CollocatedVectorGrid2();

    virtual ~CollocatedVectorGrid2();

    virtual Vector2<size_t> dataSize() const = 0;


    virtual Vector2<double> dataOrigin() const = 0;

    T& operator()(size_t i, size_t j);

    double divergenceAtDataPoint(size_t i, size_t j);

    double curlAtDataPoint(size_t i, size_t j);


    function<Vector2<double>(size_t, size_t)> dataPosition() const;

    T sample(const Vector2<double>& x) const;

    double divergence(const Vector2<double>& x) const override;

    double curl(const Vector2<double>& x) const override;

    //这里应该用什么类型？
    std::function<T(const Vector2<double>&)> sampler() const override;

    Array2Ptr<T>& datas();

    void fill(const T& value);


protected:
    
    LinearArraySampler2Ptr<T> _linearSampler;

    //暂时不实现
    //void swapCollocatedVectorGrid(CollocatedVectorGrid2* other);

    //暂时不实现
    //void setCollocatedVectorGrid(const CollocatedVectorGrid2& other);

    //暂时不实现
    //void getData(std::vector<double>* data)  override;

    //暂时不实现
    //void setData(const std::vector<double>& data) override;
    Array2Ptr<T> _data;

    virtual void onResize(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin,
        const T& initialValue) final;
private:

    //暂时不实现
    //void swap(CollocatedVectorGrid2* other);



    //这个暂时先不实现
    //LinearArraySampler2<Vector2D, double> _linearSampler;
    std::function<T(const Vector2<double>&)> _sampler;



    //暂时不实现
    //void resetSampler();
};

template<class T>
using CollocatedVectorGrid2Ptr = shared_ptr<CollocatedVectorGrid2<T>>;



//这个地方十分不理解
//就是说，对于类成员变量来说，在构造函数执行完之前，他们会被初始化么
//且模板类型和普通类型是一样的么？
//template<class T>
//CollocatedVectorGrid2<T>::CollocatedVectorGrid2():
//    _linearSampler(_data, Vector2<double>(1.0, 1.0), Vector2<double>(0.0,0.0))
//{
//    //LinearArraySampler2<T> sam(_data, Vector2<double>(1.0, 1.0), Vector2<double>());
//}


template<class T>
CollocatedVectorGrid2<T>::CollocatedVectorGrid2() 
{
   
}

template<class T>
CollocatedVectorGrid2<T>::~CollocatedVectorGrid2() {
}


template<class T>

T& CollocatedVectorGrid2<T>::operator()(size_t i, size_t j) {
    return _data(i, j);
}

template<class T>
double CollocatedVectorGrid2<T>::divergenceAtDataPoint(size_t i, size_t j) {
    return 0.0;
}


template<class T>
double CollocatedVectorGrid2<T>::curlAtDataPoint(size_t i, size_t j) {
    return 0.0;
}

template<class T>
function<Vector2<double>(size_t, size_t)> CollocatedVectorGrid2<T>::dataPosition() const {
    Vector2<double> dataOrigin_ = dataOrigin();
    return [this, dataOrigin_](double i, double j) -> Vector2<double> {
        return dataOrigin_ + this->gridSpacing() * Vector2<double>({ i, j });
    };
}


//注意这里sampler函数的实现
//这里可以使用函数对象进行优化
//线性采样器这里写的十分不好，不过可以凑活使用
//算法设计完之后再来优化程序吧，妈的，太累了
template<class T>
T CollocatedVectorGrid2<T>::sample(const Vector2<double>& x) const {

    auto _sample = _linearSampler;
    auto result = (*_sample)(this->_data, x);
    return result;
    //return _sampler(x);
}


template<class T>
double CollocatedVectorGrid2<T>::divergence(const Vector2<double>& x) const {
    return 0.0;
}


template<class T>
double CollocatedVectorGrid2<T>::curl(const Vector2<double>& x) const {
    return 0.0;
}


template<class T>
std::function<T(const Vector2<double>&)> CollocatedVectorGrid2<T>::sampler() const {
    return _sampler;
}

template<class T>
Array2Ptr<T>& CollocatedVectorGrid2<T>::datas() {
    return _data;
}

//这里可以用并行优化吗
template<class T>
void CollocatedVectorGrid2<T>::fill(const T& value) {
    auto size = dataSize();
    for (size_t i = 0; i < size.x; ++i) {
        for (size_t j = 0; j < size.y; ++j) {
            _data(i, j) = value;
        }
    }
}

template<class T>
void CollocatedVectorGrid2<T>::onResize(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    const T& initialValue) {

    _data.reSize(dataSize().x, dataSize().y, initialValue);
    //resetSampler();


}




#endif
