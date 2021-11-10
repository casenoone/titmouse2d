#ifndef VERTEXCENTEREDSCALARGRID2_H
#define VERTEXCENTEREDSCALARGRID2_H

#include "ScalarGrid2.h"

class VertexCenteredScalarGrid2 final : public ScalarGrid2 {

public:

    //类的声明
    class Builder;

    VertexCenteredScalarGrid2();

    VertexCenteredScalarGrid2(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin,
        double initialValue);

    VertexCenteredScalarGrid2(const VertexCenteredScalarGrid2& other);

    Vector2<size_t> dataSize() const override;

    Vector2<double> dataOrigin() const override;



    //静态成员函数不属于任何对象，它属于类
    //思考一下啊，如果这里的builder函数不是静态的，只是一个普通的成员函数
    //那么在外部该怎么调用它呢，那就只能通过先定义一个vertex...类对象，那这就没有必要了
    //且一定程度上破坏了抽象的概念

    //如果声明成一个静态函数，那么只需要通过类名即可调用
    //太棒了！
    static Builder builder();

    //拷贝构造函数
    //暂时不实现
    //VertexCenteredScalarGrid2& operator=(
        //const VertexCenteredScalarGrid2& other);
    
    //暂时不实现
    //std::shared_ptr<ScalarGrid2> clone() const override;
};


typedef std::shared_ptr<VertexCenteredScalarGrid2> VertexCenteredScalarGrid2Ptr;

class VertexCenteredScalarGrid2::Builder final : public ScalarGridBuilder2 {
public:
    Builder& withResolution(const Vector2<size_t>& resolution);

    Builder& withResolution(size_t resolutionX, size_t resolutionY);

    Builder& withGridSpacing(const Vector2<double>& gridSpacing);

    Builder& withGridSpacing(double gridSpacingX, double gridSpacingY);

    Builder& withOrigin(const Vector2<double>& gridOrigin);

    Builder& withOrigin(double gridOriginX, double gridOriginY);

    Builder& withInitialValue(double initialVal);

    //这个build（）是什么作用
    //返回一个vetex...类对象
    VertexCenteredScalarGrid2 build() const;

    //仍然搞不懂这个玩意儿有啥用
    //明白了，就是返回一个vertex...类对象的共享指针
    VertexCenteredScalarGrid2Ptr makeShared() const;

    ScalarGrid2Ptr build(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2 <double> & gridOrigin,
        double initialVal) const override;



private:
    Vector2<size_t> _resolution;
    Vector2 <double> _gridSpacing;
    Vector2<double> _gridOrigin;
    double _initialVal = 0.0;
};



#endif