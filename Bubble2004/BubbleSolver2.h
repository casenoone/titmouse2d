#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"
#include "../titmouse2d/src/ConstVar.h"

static const double v_eps = 0.08;

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

	//�����Ƕ���ѧ��ⲿ��
public:
	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	void emitVortexRing();

public:
	BubbleDataPtr _bubbleData;
};


inline Vector2D BubbleSolver2::computeUSingle(const Vector2D& pos, int i)const {

	auto& position = _bubbleData->vortexPosition;
	auto vorticity = _bubbleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (v_eps * v_eps)));

}