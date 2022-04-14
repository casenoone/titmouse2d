#ifndef SCALARGRID2_H
#define SCALARGRID2_H

#include "Grid2.h"
#include "ScalarField2.h"
#include "../Array2.hpp"
#include "ArraySampler2.hpp"

class ScalarGrid2 : public ScalarField2, public Grid2 {
public:

	using  ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;

	ScalarGrid2();

	//只清除数据
	virtual void clearData(double initialValue);

	virtual ~ScalarGrid2();

	virtual Vector2I dataSize()const = 0;

	virtual Vector2D dataOrigin()const = 0;

	//暂时不实现
	//virtual std::shared_ptr<ScalarGrid2> clone() const = 0;

	//返回给定下标的数据
	double& operator()(int i, int j);

	double lookAt(int i, int j)const;

	//返回给定点的梯度算子结果
	Vector2D gradientAtDataPoint(int i, int j) const;

	double laplacianAtDataPoint(int i, int j) const;


	using DataPositionFunc = std::function<Vector2D(int, int)>;


	DataPositionFunc dataPosition() const;

	double sample(const Vector2D& x) const override;

	std::function<double(const Vector2D&)> sampler() const override;

	Vector2D gradient(const Vector2D& x) const override;

	double laplacian(const Vector2D& x) const override;

	void fill(double value);

	void resize(const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin,
		double initialValue = 0.0);

	Array2D& datas();

	const Array2D datas() const;



	//void copyTo()const;

protected:
	Array2D _data;

	LinearArraySampler2Ptr<double> _linearSampler;
};



using  ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;





class ScalarGridBuilder2 {
public:

	ScalarGridBuilder2();

	virtual ~ScalarGridBuilder2();

	virtual ScalarGrid2Ptr build(
		const Vector2I& resolution,
		const Vector2D& gridOrigin,
		double initialVal) const = 0;
};



#endif