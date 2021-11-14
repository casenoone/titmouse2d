#ifndef FACECENTEREDGRID2_H
#define FACECENTEREDGRID2_H

#include "VectorGrid2.h"
#include "../Array2Ptr.hpp"
#include "../Eulerian/ArraySampler2.hpp"


class FaceCenteredGrid2 final : public VectorGrid2 {

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

class FaceCenteredGrid2::Builder final : public VectorGridBuilder2 {
public:
	Builder& withResolution(const Vector2<size_t>& resolution);

	Builder& withResolution(size_t resolutionX, size_t resolutionY);

	Builder& withOrigin(const Vector2<double>& gridOrigin);

	Builder& withOrigin(double gridOriginX, double gridOriginY);

	Builder& withInitialValue(const Vector2<double>& initialVal);

	Builder& withInitialValue(double initialValX, double initialValY);

	FaceCenteredGrid2 build() const;

	FaceCenteredGrid2Ptr makeShared() const;

	VectorGrid2Ptr build(const Vector2<size_t>& resolution,
		const Vector2<double>& gridOrigin,
		const Vector2<double>& initialVal) const override;

private:
	Vector2<size_t> _resolution;
	Vector2<double> _gridOrigin;
	Vector2<double> _gridSpacing;
 	Vector2<double> _initialVal;
};

#endif