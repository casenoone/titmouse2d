#ifndef VERTEXCENTEREDSCALARGRID2_H
#define VERTEXCENTEREDSCALARGRID2_H

#include "ScalarGrid2.h"

class VertexCenteredScalarGrid2 final : public ScalarGrid2 {

public:

	//�������
	class Builder;

	VertexCenteredScalarGrid2();

	VertexCenteredScalarGrid2(
		const Vector2I& resolution,
		const Vector2D& origin = Vector2D(),
		double initialValue = 0.0);



	VertexCenteredScalarGrid2(const VertexCenteredScalarGrid2& other);

	Vector2I dataSize() const override;

	Vector2D dataOrigin() const override;

	void clearData(double initialValue)override;

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
	Builder& withResolution(const Vector2I& resolution);

	Builder& withResolution(int resolutionX, int resolutionY);

	Builder& withOrigin(const Vector2D& gridOrigin);

	Builder& withOrigin(double gridOriginX, double gridOriginY);

	Builder& withInitialValue(double initialVal);


	//����һ��vetex...�����
	VertexCenteredScalarGrid2 build() const;

	//����һ��vertex...�����Ĺ���ָ��
	VertexCenteredScalarGrid2Ptr makeShared() const;

	ScalarGrid2Ptr build(
		const Vector2I& resolution,
		const Vector2 <double>& gridOrigin,
		double initialVal) const override;



private:
	Vector2I _resolution;
	Vector2D _gridOrigin;
	double _initialVal = 0.0;
};



#endif