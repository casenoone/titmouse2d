#ifndef LBMSOLVER2_H
#define LBMSOLVER2_H

#include <iostream>
using namespace std;

#include "LBMData2.h"

#include <array>

const int LBM_OBS = 0;
const int LBM_FLUID = 1;
const int LBM_VELOCITY = 2;

#include "../../Eulerian/VertexCenteredScalarGrid2.h"

class LBMSolver2 {
public:

	LBMSolver2() = default;

	LBMSolver2(const Vector2<int>& resolution);

	Vector2<int> resolution()const;

	void setCollider(const Array2Ptr<int>& collider);

	void onAdvancedTimeStep();

	Array2Ptr<double> getRho()const;

	const Vector2<double> velocityAt(int i, int j)const;


	int getGridState(int i, int j)const;

private:

	void stream();

	void collide();

	auto getDensityVelocity(int i, int j);

public:
	LBMData2 _data;

private:
	Array2<int> _g;
};







#endif