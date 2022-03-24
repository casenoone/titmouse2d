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

	//暂时不实现
	//void swap(Grid2* other) override;

	//暂时不实现
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

	//这里想一个更好的实现方式
	Array2D& uDatas();
	Array2D& vDatas();

	Vector2D uOrigin() const;

	Vector2D vOrigin() const;

	Vector2D sample(const Vector2D& x)  const override;

	double divergence(const Vector2D& x) const override;

	double curl(const Vector2D& x) const override;

	std::function<Vector2D(const Vector2D&)> sampler() const override;

	//暂时不实现
	//std::shared_ptr<VectorGrid2> clone() const override;

	void fill(const Vector2D& value)override;


	//由于faceCentered常常被用作速度场
	//在求解线性系统时，往往只求解流体所在部分
	//所以用该成员去标识线性系统中未知量系数的序号
	//我们令其初始化为空气
	Array2I solveSystemMarker;

	static Builder builder();

protected:
	void onResize(const Vector2I& resolution, const Vector2D& gridSpacing,
		const Vector2D& origin, const Vector2D& initialValue) final;


	//用到时再实现
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