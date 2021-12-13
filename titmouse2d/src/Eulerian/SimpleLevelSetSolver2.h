#ifndef SIMPLELEVELSETSOLVER2_H
#define SIMPLELEVELSETSOLVER2_H

#include "VertexCenteredScalarGrid2.h"

//������Ϊ���ž��볡���Ǵ���VertexCentered������

//�����level set�������һ��ӭ���ʽ�������

class SimpleLevelSetSolver2 {
public:
	SimpleLevelSetSolver2();

	~SimpleLevelSetSolver2();

	void reinitialize(const VertexCenteredScalarGrid2& inputSdf, double maxDistance,
		VertexCenteredScalarGrid2Ptr& outputSdf);

private:
	double sign(const Array2Ptr<double>& input, int i, int j) const;

	double pseudoTimeStep(const Array2Ptr<double>& input,
		const Vector2<double>& gridSpacing);

	int distanceToNumberOfIterations(double distance, double dtau);

	void getDerivatives(const Array2Ptr<double>& input, Vector2<double> gridSpacing,
		size_t i, size_t j, std::array<double, 2>* dx, std::array<double, 2>* dy) const;

private:
	double _maxCfl = 0.5;
};

typedef std::shared_ptr<SimpleLevelSetSolver2> SimpleLevelSetSolver2Ptr;



#endif