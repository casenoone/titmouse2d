#ifndef FOAMVORTEXSOLVER2_H
#define FOAMVORTEXSOLVER2_H


#include "FoamVortexData.h"
#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/OtherMethod/SWE/ShallowWaveSolver2.h"
#include "../titmouse2d/src/boundingbox2.h"


#include <Eigen/Dense>


//坐标规定：
//x : pos.x
//y : height
//z : pos.y


class FoamVortexSolver : public ParticleSystemSolver2 {

public:
	FoamVortexSolver();

	//高度场与涡粒子耦合专用
	FoamVortexSolver(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin = Vector2D::zero());

	virtual void timeIntegration(double timeIntervalInSeconds)override;
	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;
	FoamVortexDataPtr& foamVortexData();
	Vector2D computeUSingle(const Vector2D& pos, int i)const;
	void setMovingBoudnary(RegularPolygonPtr surfaces);
	void setStaticBoudnary(ExplicitSurface2Ptr surfaces);
	//void 
	void emitTracerParticles();
	void emitParticlesFromPanels(double timeIntervalInSeconds);
	void emitVortexRing();
	//为高度场设置移动边界
	void setShallowWaveMovingBoundary(const Vector2D& center, const double r);

private:
	//求解tracer粒子
	void tracerParticlesSolve();

	//只要边界形状不变，边界矩阵就不会变
	//这个函数只调用一次
	void constructMovingBoundaryMatrix();

	void constructStaticBoundaryMatrix();

	//我也不想起这么长的名字
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);
	Vector2D static_computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);
	void no_through_solve(double timeIntervalInSeconds);
	Vector2D static_computeSingleVelocityFromPanels(int index);
public:
	ShallowWaveSolver2Ptr _shallowWaveSolver;

private:
	FoamVortexDataPtr _foamVortexData;
};



#endif
