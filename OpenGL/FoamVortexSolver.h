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

	//Ϊ�߶ȳ������ƶ��߽�

private:

	//���tracer����
	void tracerParticlesSolve();


	//ֻҪ�߽���״���䣬�߽����Ͳ����
	//�������ֻ����һ��
	void computeBoundaryMatrix();

	//��Ҳ��������ô��������
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();


private:
	FoamVortexDataPtr _foamVortexData;

	ShallowWaveSolver2Ptr _shallowWaveSolver;

};



#endif
