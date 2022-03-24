#ifndef FACECENTEREDGRID2_H
#define FACECENTEREDGRID2_H

#include "VectorGrid2.hpp"
#include "../Array2.hpp"
#include "../Eulerian/ArraySampler2.hpp"

class FaceCenteredGrid2 final : public VectorGrid2<Vector2D> {

public:
	class Builder;

	FaceCenteredGrid2();

	FaceCenteredGrid2(
		const Vector2I& resolution,
		const Vector2D& origin,
		const Vector2D& initialValue);

	FaceCenteredGrid2(const FaceCenteredGrid2& other);

	//��ʱ��ʵ��
	//void swap(Grid2* other) override;

	//��ʱ��ʵ��
	//void set(const FaceCenteredGrid2& other);

	FaceCenteredGrid2& operator=(const FaceCenteredGrid2& other);

	double& u(int i, int j);

	double& v(int i, int j);

	Vector2D valueAtCellCenter(int i, int j)const;

	double divergenceAtCellCenter(int i, int j);

	double curlAtCellCenter(int i, int j)const;

	DataPositionFunc uPosition() const;

	DataPositionFunc vPosition() const;

	Size2 uSize();

	Size2 vSize();

	//������һ�����õ�ʵ�ַ�ʽ
	Array2D& uDatas();
	Array2D& vDatas();

	Vector2D uOrigin() const;

	Vector2D vOrigin() const;

	Vector2D sample(const Vector2D& x)  const override;

	double divergence(const Vector2D& x) const override;

	double curl(const Vector2D& x) const override;

	std::function<Vector2D(const Vector2D&)> sampler() const override;

	//��ʱ��ʵ��
	//std::shared_ptr<VectorGrid2> clone() const override;

	void fill(const Vector2D& value)override;


	//����faceCentered�����������ٶȳ�
	//���������ϵͳʱ������ֻ����������ڲ���
	//�����øó�Աȥ��ʶ����ϵͳ��δ֪��ϵ�������
	//���������ʼ��Ϊ����
	Array2I solveSystemMarker;

	static Builder builder();

protected:
	void onResize(const Vector2I& resolution, const Vector2D& gridSpacing,
		const Vector2D& origin, const Vector2D& initialValue) final;


	//�õ�ʱ��ʵ��
	//void getData(std::vector<double>* data)  override;

	//void setData(const std::vector<double>& data) override;


private:
	Array2D _dataU;
	Array2D _dataV;
	Vector2D _dataOriginU;
	Vector2D _dataOriginV;

	LinearArraySampler2<double> _uLinearSampler;
	LinearArraySampler2<double> _vLinearSampler;

	std::function<Vector2D(const Vector2D&)> _sampler;

	void resetSampler();

};

typedef std::shared_ptr<FaceCenteredGrid2> FaceCenteredGrid2Ptr;


#endif