#ifndef LEVELSETLIQUIDSOLVER2_H
#define LEVELSETLIQUIDSOLVER2_H

#include "GridFluidSolver2.h"
class LevelSetLiquidSolver2 : public GridFluidSolver2 {
public:
	LevelSetLiquidSolver2();

	~LevelSetLiquidSolver2();

	void extrapolateVelocityToAir();
};



#endif