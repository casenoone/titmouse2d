#ifndef LEVELSETLIQUIDSOLVER2_H
#define LEVELSETLIQUIDSOLVER2_H

#include "GridFluidSolver2.h"
#include "SimpleLevelSetSolver2.h"

class LevelSetLiquidSolver2 : public GridFluidSolver2 {
public:
	
	class Builder;
	
	LevelSetLiquidSolver2();

	~LevelSetLiquidSolver2();

	LevelSetLiquidSolver2(const Vector2<size_t>& resolution, 
		const Vector2<double>& gridSpacing, 
		const Vector2<double>& gridOrigin);

	void extrapolateVelocityToAir();

	static Builder builder();

private:
	void setMarkers();
	
	void computeSdfAdvection(double timeIntervalInSeconds);

	virtual void onBeginAdvanceTimeStep(double timeIntervalInSeconds)override;

	virtual void onEndAdvanceTimeStep(double timeIntervalInSeconds)override;


private:

	SimpleLevelSetSolver2Ptr _levelsetSolver;
};

typedef std::shared_ptr<LevelSetLiquidSolver2> LevelSetLiquidSolver2Ptr;



class LevelSetLiquidSolver2::Builder final
	: public GridFluidSolverBuilderBase2<LevelSetLiquidSolver2::Builder> {
public:
	LevelSetLiquidSolver2 build() const;

	LevelSetLiquidSolver2Ptr makeShared() const;
};


#endif