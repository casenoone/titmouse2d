#ifndef LBMSOLVER2_H
#define LBMSOLVER2_H

#include <iostream>
using namespace std;

#include "LBMData2.h"

#include <array>

const int OBS = 0;
const int FLUID = 1;


class LBMSolver2 {
public:
	LBMSolver2() = default;

	Vector2<int> resolution()const;

	void setData();

	void initLBM();

	void stream();

	void collide();

	void onAdvancedTimeStep();
private:
	auto getDensityVelocity(int i, int j);

public:
	LBMData2 _data;

private:
	Array2<int> _g;
};


Vector2<int> LBMSolver2::resolution()const {
	return _data.resolution();
}




#endif