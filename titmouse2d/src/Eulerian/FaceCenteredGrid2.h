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

	//暂时不实现
	//void swap(Grid2* other) override;

	//暂时不实现
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

	//这里想一个更好的实现方式
	Array2Ptr<double>& uDatas();
	Array2Ptr<double>& vDatas();

	Vector2<double> uOrigin() const;

	Vector2<double> vOrigin() const;

	Vector2<double> sample(const Vector2<double>& x)  const override;

	double divergence(const Vector2<double>& x) const override;

	double curl(const Vector2<double>& x) const override;

	std::function<Vector2<double>(const Vector2<double>&)> sampler() const override;

	//暂时不实现
	//std::shared_ptr<VectorGrid2> clone() const override;

	void fill(const Vector2<double>& value)override;


	//由于faceCentered常常被用作速度场
	//在求解线性系统时，往往只求解流体所在部分
	//所以用该成员去标识线性系统中未知量系数的序号
	//我们令其初始化为空气
	Array2Ptr<int> solveSystemMarker;

	static Builder builder();

protected:
	void onResize(const Vector2<size_t>& resolution, const Vector2<double>& gridSpacing,
		const Vector2<double>& origin, const Vector2<double>& initialValue) final;


	//用到时再实现
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