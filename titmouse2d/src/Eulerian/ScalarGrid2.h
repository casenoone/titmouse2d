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

	//ֻ�������
	virtual void clearData(double initialValue);

	virtual ~ScalarGrid2();

	virtual Vector2<int> dataSize()const = 0;

	virtual Vector2<double> dataOrigin()const = 0;

	//��ʱ��ʵ��
	//virtual std::shared_ptr<ScalarGrid2> clone() const = 0;

	//���ظ����±������
	double& operator()(int i, int j);

	double lookAt(int i, int j)const;

	//���ظ�������ݶ����ӽ��
	Vector2<double> gradientAtDataPoint(int i, int j) const;

	double laplacianAtDataPoint(int i, int j) const;


	using DataPositionFunc = function<Vector2<double>(int, int)>;


	DataPositionFunc dataPosition() const;

	double sample(const Vector2<double>& x) const override;

	std::function<double(const Vector2<double>&)> sampler() const override;

	Vector2<double> gradient(const Vector2<double>& x) const override;

	double laplacian(const Vector2<double>& x) const override;

	void fill(double value);

	void resize(const Vector2<int>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& origin,
		double initialValue = 0.0);

	Array2<double>& datas();

	const Array2<double> datas() const;



	//void copyTo()const;

protected:
	Array2<double> _data;

	LinearArraySampler2Ptr<double> _linearSampler;
};



using  ScalarGrid2Ptr = std::shared_ptr<ScalarGrid2>;





class ScalarGridBuilder2 {
public:

	ScalarGridBuilder2();

	virtual ~ScalarGridBuilder2();

	virtual ScalarGrid2Ptr build(
		const Vector2<int>& resolution,
		const Vector2<double>& gridOrigin,
		double initialVal) const = 0;
};



#endif