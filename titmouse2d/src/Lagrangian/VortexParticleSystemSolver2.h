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

	//��panel�����ݽṹ��һ�������Ա��ھֲ�����ϵ�µļ���
	void correctPanelCoordinateSystem();

	//ֻҪ�߽���״���䣬�߽����Ͳ����
	//�������ֻ����һ��
	void computeBoundaryMatrix();

	//��Ҳ��������ô��������
	Vector2D computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);

	Vector2D computeSingleVelocityFromPanels(int index);

	void vortexSheetSolve(double timeIntervalInSeconds);

	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();


private:
	VortexParticleSystemData2Ptr _vortexParticleData;

};



#endif