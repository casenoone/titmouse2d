#ifndef FACECENTEREDGRID2_H
#define FACECENTEREDGRID2_H

#include "VectorGrid2.hpp"
#include "../Array2Ptr.hpp"
#include "../Eulerian/ArraySampler2.hpp"

class FaceCenteredGrid2 final : public VectorGrid2<Vector2<double>> {

public:
	class Builder;

	FaceCenteredGrid2();

	FaceCenteredGrid2(const Vector2<size_t>& resolution,
		const Vector2<double>& origin ,
		const Vector2<double>& initialValue);

	FaceCenteredGrid2(const FaceCenteredGrid2& other);

	//��ʱ��ʵ��
	//void swap(Grid2* other) override;

	//��ʱ��ʵ��
	//void set(const FaceCenteredGrid2& other);

	FaceCenteredGrid2& operator=(const FaceCenteredGrid2& other);

	double& u(size_t i, size_t j);

	double& v(size_t i, size_t j);

	Vector2<double> valueAtCellCenter(size_t i, size_t j);

	double divergenceAtCellCenter(size_t i, size_t j);

	double curlAtCellCenter(size_t i, size_t j);

	DataPositionFunc uPosition() const;

	DataPositionFunc vPosition() const;

	Size2 uSize();

	Size2 vSize();

	//������һ�����õ�ʵ�ַ�ʽ
	Array2Ptr<double>& uDatas();
	Array2Ptr<double>& vDatas();

	Vector2<double> uOrigin() const;

	Vector2<double> vOrigin() const;

	Vector2<double> sample(const Vector2<double>& x)  const override;

	double divergence(const Vector2<double>& x) const override;

	double curl(const Vector2<double>& x) const override;

	std::function<Vector2<double>(const Vector2<double>&)> sampler() const override;

	//��ʱ��ʵ��
	//std::shared_ptr<VectorGrid2> clone() const override;

	void fill(const Vector2<double>& value)override;


	//����faceCentered�����������ٶȳ�
	//���������ϵͳʱ������ֻ����������ڲ���
	//�����øó�Աȥ��ʶ����ϵͳ��δ֪��ϵ�������
	//���������ʼ��Ϊ����
	Array2Ptr<int> solveSystemMarker;

	static Builder builder();

protected:
	void onResize(const Vector2<size_t>& resolution, const Vector2<double>& gridSpacing,
		const Vector2<double>& origin, const Vector2<double>& initialValue) final;


	//�õ�ʱ��ʵ��
	//void getData(std::vector<double>* data)  override;

	//void setData(const std::vector<double>& data) override;


private:
	Array2Ptr<double> _dataU;
	Array2Ptr<double> _dataV;
	Vector2<double> _dataOriginU;
	Vector2<double> _dataOriginV;

	LinearArraySampler2<double> _uLinearSampler;
	LinearArraySampler2<double> _vLinearSampler;

	std::function<Vector2<double>(const Vector2<double>&)> _sampler;

	void resetSampler();

};

typedef std::shared_ptr<FaceCenteredGrid2> FaceCenteredGrid2Ptr;


#endif