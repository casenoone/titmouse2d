#ifndef VORTEXPARTICLESSYSTEMSOLVER2_H
#define VORTEXPARTICLESSYSTEMSOLVER2_H

#include "VortexParticleSystemData2.h"
#include "ParticleSystemSolver2.h"

#include <Eigen/Dense>


class VortexParticleSystemSolver2 : public ParticleSystemSolver2 {

public:
	VortexParticleSystemSolver2();

	virtual ~VortexParticleSystemSolver2();

	virtual void timeIntegration(double timeIntervalInSeconds)override;


	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	VortexParticleSystemData2Ptr& vortexParticleData();

	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	void setData(int numberOfParticles,
		Array<Vector2D>& pos,
		int resolutionX,
		int resolutionY);

	void setPanels(ExplicitSurface2Ptr surfaces);

	void emitParticlesFromPanel();

private:

	//对panel的数据结构做一个矫正以便于局部坐标系下的计算
	void correctPanelCoordinateSystem();

	//只要边界形状不变，边界矩阵就不会变
	//这个函数只调用一次
	void computeBoundaryMatrix();

	//我也不想起这么长的名字
	Vector2D computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);

	Vector2D computeSingleVelocityFromPanels(int index);

	void vortexSheetSolve(double timeIntervalInSeconds);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();


private:
	VortexParticleSystemData2Ptr _vortexParticleData;

};



#endif