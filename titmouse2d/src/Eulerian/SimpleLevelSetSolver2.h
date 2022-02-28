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
	double sign(const Array2D& input, int i, int j) const;

	double pseudoTimeStep(const Array2D& input,
		const Vector2D& gridSpacing);

	int distanceToNumberOfIterations(double distance, double dtau);

	void getDerivatives(const Array2D& input, Vector2D gridSpacing,
		int i, int j, std::array<double, 2>* dx, std::array<double, 2>* dy) const;

private:
	double _maxCfl = 0.5;
};

typedef std::shared_ptr<SimpleLevelSetSolver2> SimpleLevelSetSolver2Ptr;



#endif