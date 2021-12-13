#ifndef SCALARGRID2_H
#define SCALARGRID2_H

#include "Grid2.h"
#include "ScalarField2.h"
#include "../Array2Ptr.hpp"
#include "ArraySampler2.hpp"

class ScalarGrid2 : public ScalarField2, public Grid2 {
public:

	using  ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;

	ScalarGrid2();

	//只清除数据
	virtual void clearData(double initialValue);

	virtual ~ScalarGrid2();

	virtual Vector2<size_t> dataSize()const = 0;
	
	virtual Vector2<double> dataOrigin()const = 0;

	//暂时不实现
	//virtual std::shared_ptr<ScalarGrid2> clone() const = 0;

	//返回给定下标的数据
	double& operator()(size_t i, size_t j);

	double lookAt(size_t i, size_t j)const;

	//返回给定点的梯度算子结果
	Vector2<double> gradientAtDataPoint(size_t i, size_t j) const;

	double laplacianAtDataPoint(size_t i, size_t j) const;
	

	using DataPositionFunc = function<Vector2<double>(size_t, size_t)>;
	

	DataPositionFunc dataPosition() const;

	double sample(const Vector2<double>& x) const override;

	std::function<double(const Vector2<double>&)> sampler() const override;

	Vector2<double> gradient(const Vector2<double>& x) const override;

	double laplacian(const Vector2<double>& x) const override;

	void fill(double value);

	void resize(const Vector2<size_t>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& origin ,
		double initialValue = 0.0);

	Array2Ptr<double>& datas();

	const Array2Ptr<double> datas() const;

	

	//void copyTo()const;

protected:
	Array2Ptr<double> _data;

	LinearArraySampler2Ptr<double> _linearSampler;
};



using  ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;





class ScalarGridBuilder2 {
public:

	ScalarGridBuilder2();

	virtual ~ScalarGridBuilder2();

	virtual ScalarGrid2Ptr build(
		const Vector2<size_t>& resolution,
		const Vector2<double>& gridOrigin,
		double initialVal) const = 0;
};



#endif