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

	//ʱ�����
	virtual void timeIntegration(double timeIntervalInSeconds)override;

	//ʱ������
	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	FoamVortexDataPtr& foamVortexData();

	//���������Ӷ��������ӵ��յ��ٶ�
	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	//�����ƶ��߽磨ʹ��siggraph2020�ı߽���ⷽ����
	void setMovingBoudnary(RegularPolygonPtr surfaces);

	//���þ�̬�߽磨ʹ��siggraph2005�ı߽���ⷽ����
	void setStaticBoudnary(ExplicitSurface2Ptr surfaces);

	//����׷������
	void emitTracerParticles();

	//���ƶ��߽��Ϸ���������������no-slip�߽�����
	void emitParticlesFromPanels(double timeIntervalInSeconds);

	//����vortex ring
	void emitVortexRing();

	//Ϊ�߶ȳ������ƶ��߽�
	void setShallowWaveMovingBoundary(const Vector2D& center, const double r);

	//�������������ݵĽ�����
	Vector2D computeTwoWayForce(int index, double dt);

	//�������ݶ�Ӧ��panelset
	void generatePanelSet(const Array<Vector2D>& pos, const Array<double>& radius);


private:
	//���tracer����
	void tracerParticlesSolve();

	//ֻҪ�߽���״���䣬�߽����Ͳ����
	//�ƶ��߽����߽���ַ���ʱʹ�õľ���SIGGRAPH2020��
	void constructMovingBoundaryMatrix();

	//��̬�߽����߽���ַ���ʱʹ�õľ���SIGGRAPH2005��
	void constructStaticBoundaryMatrix();

	//��װ���two-way����ʱ�õ��ľ���
	void constructTwoWayBoundaryMatrix();

	//���������ĭ���ϵ�vortex strength
	void all_bubble_vortexStrengthSolve(double dt);

	//��װ�ƶ��߽��Ӧ�ľ�������
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);

	//��װ��̬�߽��Ӧ�ľ�������
	Vector2D static_computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);

	//��װtwoway��Ӧ�ľ�������
	Vector2D two_way_computeUnitVelocityFromPanels(int bubble_idx, int panelIdx, const Vector2D& midPoint);

	//��⾲̬�߽����㲻��͸�߽������µ�vortex_strength
	void no_through_solve(double timeIntervalInSeconds);

	//��̬�߽��ڿռ���ĳ�����յ��ٶ�
	Vector2D static_computeSingleVelocityFromPanels(int index);

	//���׷�����ӵı߽紩͸����
	void tarcerCollisionSolve(Vector2D& pos);

	//������ɢ������ɷ����һЩ��΢����һ��ĺ�ɢģ�ͣ�
	void decayVorticity();

	//����bubble_panelset��λ��
	void update_bubble_panelset_pos(double dt);

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

	//�������е�two-way��
	void compute_all_twoway_force(double dt);

	//�������е���
	void computeTotalForce(double dt);

	//������ʧ�����ɾ������
	void bubbleBreakUp();

public:
	ShallowWaveSolver2Ptr _shallowWaveSolver;

private:
	FoamVortexDataPtr _foamVortexData;
};



#endif
