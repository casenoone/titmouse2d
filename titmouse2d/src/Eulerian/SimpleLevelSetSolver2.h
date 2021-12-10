#ifndef SIMPLELEVELSETSOLVER2_H
#define SIMPLELEVELSETSOLVER2_H

#include "VertexCenteredScalarGrid2.h"

//这里认为符号距离场总是存于VertexCentered网格里

//这里的level set求解器是一个迎风格式的求解器

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