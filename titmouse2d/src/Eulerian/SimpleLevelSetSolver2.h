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

	void getDerivatives(const VertexCenteredScalarGrid2& inputSdf, Vector2<size_t> gridSpacing,
		size_t i, size_t j, std::array<double, 2>* arr);

private:
	double sign(const VertexCenteredScalarGrid2& inputSdf, int i, int j) const;

private:
	int numberOfIterations = 20;

};


#endif