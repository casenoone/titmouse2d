//#ifndef FOAMVORTEXSOLVER_H
//#define FOAMVORTEXSOLVER_H
//
//
//#include "FoamVortexData.h"
//#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
//
//#include "../titmouse2d/src/boundingbox2.h"
//
//
//#include <Eigen/Dense>
//
//
//class FoamVortexSolver : public ParticleSystemSolver2 {
//
//public:
//	FoamVortexSolver();
//
//	virtual ~FoamVortexSolver();
//
//	virtual void timeIntegration(double timeIntervalInSeconds)override;
//
//
//	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;
//
//	FoamVortexDataPtr& foamVortexData();
//
//	Vector2D computeUSingle(const Vector2D& pos, int i)const;
//
//	void setData(int numberOfParticles,
//		Array<Vector2D>& pos,
//		int resolutionX,
//		int resolutionY);
//
//	void setPanels(ExplicitSurface2Ptr surfaces);
//
//	void setMovingGrid(const Vector2I& resolution_, const BoundingBox2& domain_);
//
//	void emitParticles();
//
//	void emitParticlesFromPanels(double timeIntervalInSeconds);
//
//
//
//private:
//
//	//求解tracer粒子
//	void tracerParticlesSolve();
//
//	Vector2D computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);
//
//	//对panel的数据结构做一个矫正以便于局部坐标系下的计算
//	void correctPanelCoordinateSystem();
//
//	//只要边界形状不变，边界矩阵就不会变
//	//这个函数只调用一次
//	void computeBoundaryMatrix();
//
//	//我也不想起这么长的名字
//	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);
//
//	Vector2D computeSingleVelocityFromPanels(int index);
//
//	void vortexSheetSolve(double timeIntervalInSeconds);
//
//	void slipVortexSheetSolve(double timeIntervalInSeconds);
//
//	void onBeginAdvanceTimeStep();
//
//	void onEndAdvanceTimeStep();
//
//
//private:
//	FoamVortexDataPtr _foamVortexData;
//
//};
//
//
//
//#endif