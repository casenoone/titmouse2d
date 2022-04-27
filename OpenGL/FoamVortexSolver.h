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
		const Vector2D& gridOrigin);

	virtual ~FoamVortexSolver();

	virtual void timeIntegration(double timeIntervalInSeconds)override;


	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	FoamVortexDataPtr& foamVortexData();

	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	void setData(int numberOfParticles,
		Array<Vector2D>& pos,
		int resolutionX,
		int resolutionY);

	void setPanels(RegularPolygonPtr surfaces);

	void setMovingGrid(const Vector2I& resolution_, const BoundingBox2& domain_);

	void emitParticles();

	void emitTracerParticles();

	void emitParticlesFromPanels(double timeIntervalInSeconds);

	//为高度场设置移动边界

private:

	//求解tracer粒子
	void tracerParticlesSolve();


	//只要边界形状不变，边界矩阵就不会变
	//这个函数只调用一次
	void computeBoundaryMatrix();

	//我也不想起这么长的名字
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();


private:
	FoamVortexDataPtr _foamVortexData;

	ShallowWaveSolver2Ptr _shallowWaveSolver;

};



#endif
