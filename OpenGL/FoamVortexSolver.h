#ifndef FOAMVORTEXSOLVER2_H
#define FOAMVORTEXSOLVER2_H


#include "FoamVortexData.h"
#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/OtherMethod/SWE/ShallowWaveSolver2.h"
#include "../titmouse2d/src/boundingbox2.h"


#include <Eigen/Dense>


//����涨��
//x : pos.x
//y : height
//z : pos.y


class FoamVortexSolver : public ParticleSystemSolver2 {

public:
	FoamVortexSolver();

	//�߶ȳ������������ר��
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
	//Ϊ�߶ȳ������ƶ��߽�
	void setShallowWaveMovingBoundary(const Vector2D& center, const double r);

private:
	//���tracer����
	void tracerParticlesSolve();

	//ֻҪ�߽���״���䣬�߽����Ͳ����
	//�������ֻ����һ��
	void constructMovingBoundaryMatrix();

	void constructStaticBoundaryMatrix();

	//��Ҳ��������ô��������
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
