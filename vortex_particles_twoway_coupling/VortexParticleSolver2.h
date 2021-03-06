#pragma once


#include "VortexParticleData2.h"
#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"

#include "../titmouse2d/src/boundingbox2.h"


#include <Eigen/Dense>


class FoamVortexSolver : public ParticleSystemSolver2 {

public:
	FoamVortexSolver();

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
	void emitVortexRing();
	void emitParticlesFromPanels(double timeIntervalInSeconds);

	Vector2D computeTwoWayForce(double timeIntervalInSeconds);

	Vector2D computeTwoWayForce(int index, double dt);

	void generatePanelSet(const Array<Vector2D>& pos, const Array<double>& radius);

private:

	//求解tracer粒子
	void tracerParticlesSolve();

	Vector2D computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);


	//只要边界形状不变，边界矩阵就不会变
	//这个函数只调用一次
	void computeBoundaryMatrix();

	//我也不想起这么长的名字
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);

	Vector2D computeSingleVelocityFromPanels(int index);

	void vortexSheetSolve(double timeIntervalInSeconds);

	void slipVortexSheetSolve(double timeIntervalInSeconds);

	void movingSlipVortexSheetSolve(double timeIntervalInSeconds);

	void all_movingSlipVortexSheetSolve(double timeIntervalInSeconds);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();



private:
	FoamVortexDataPtr _foamVortexData;

};


