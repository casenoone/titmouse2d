#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"

class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	void timeIntegration(double timeIntervalInSeconds);

	void onAdvanceTimeStep(double timeIntervalInSeconds);

private:
	//����bubble֮����ų���
	Vector2D computeF_rB(int i, int j) const;

	//����bubble֮���������
	Vector2D computeF_aB(int i, int j) const;

	//�����ϰ����bubble��������
	void computeF_a0();

	//�������е����������ų���
	void computeF_ra();

	//�����������
	Vector2D computeF_air(int i);

	//�������е�����
	void computeF_fr();

	//�������е���
	void computeTotalForce();

	//������ʧ�����ɾ������
	void bubbleBreakUp();

public:
	BubbleDataPtr _bubbleData;
};
