#ifndef VERTEXCENTEREDSCALARGRID2_H
#define VERTEXCENTEREDSCALARGRID2_H

#include "ScalarGrid2.h"

class VertexCenteredScalarGrid2 final : public ScalarGrid2 {

public:

    //�������
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



    //��̬��Ա�����������κζ�����������
    //˼��һ�°�����������builder�������Ǿ�̬�ģ�ֻ��һ����ͨ�ĳ�Ա����
    //��ô���ⲿ����ô�������أ��Ǿ�ֻ��ͨ���ȶ���һ��vertex...����������û�б�Ҫ��
    //��һ���̶����ƻ��˳���ĸ���

    //���������һ����̬��������ôֻ��Ҫͨ���������ɵ���
    //̫���ˣ�
    static Builder builder();

    //�������캯��
    //��ʱ��ʵ��
    //VertexCenteredScalarGrid2& operator=(
        //const VertexCenteredScalarGrid2& other);
    
    //��ʱ��ʵ��
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

    //���build������ʲô����
    //����һ��vetex...�����
    VertexCenteredScalarGrid2 build() const;

    //��Ȼ�㲻������������ɶ��
    //�����ˣ����Ƿ���һ��vertex...�����Ĺ���ָ��
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