#ifndef LBMSOLVER2_H
#define LBMSOLVER2_H

#include <iostream>

#include "LBMData2.h"

#include <array>

const int LBM_OBS = 0;
const int LBM_FLUID = 1;
const int LBM_VELOCITY = 2;

#include "../../Eulerian/VertexCenteredScalarGrid2.h"

class LBMSolver2 {
public:

	LBMSolver2() = default;

	LBMSolver2(const Vector2I& resolution);

	Vector2I resolution()const;

	void setCollider(const Array2I& collider);

	void onAdvancedTimeStep();

	Array2D getRho()const;

	const Vector2D velocityAt(int i, int j)const;


	int getGridState(int i, int j)const;

private:

	void stream();

	void collide();

	auto getDensityVelocity(int i, int j);

public:
	LBMData2 _data;

private:
	Array2I _g;
};







#endif