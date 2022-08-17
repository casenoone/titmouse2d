#ifndef FOAMVORTEXSOLVER2_H
#define FOAMVORTEXSOLVER2_H


#include "FoamVortexData2.h"
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

	void setMovingBoudnary(RecTanglePtr surfaces);

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

	//Ϊ�߶ȳ����þ����ƶ��߽�
	void setShallowWaveMovingBoundary(const RecTanglePtr box);

	//�������������ݵĽ�����
	Vector2D computeTwoWayForce(int index, double dt);

	//�������ݶ�Ӧ��panelset
	void generatePanelSet(const Array<Vector2D>& pos, const Array<double>& radius);

	void generatePanelSet(const Array<Vector2D>& pos);


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
	/**************������ĭ**************/

	void bubble_timeIntegration(double dt);

	//����Compliant Matrix
	void constructCompliantMat();

	//����Jacobin Matrix
	void constructJacobinMat();

	//����ϵͳ���ʵ�֮���Լ��
	void constructConstraint();

	//������������װjacobin�е�Ԫ�أ�
	Vector2D getDerivation(int phi_idx, int x_idx);

	//���Ҫ�޸ĳɻ������������Ľṹ
	double computeConstraint(int idx);

	//���������ϵͳ�õ���constraint����
	void construct_ConstraintVector(Eigen::VectorXd& vec);

	//�����������ϵͳ�õ����ٶȳ�������
	void construct_VelocityVector(Eigen::VectorXd& vec);

	//���㵯��֮�������
	Vector2D computeSpringDragForce(int idx);

	//�������е�two-way��
	void compute_all_twoway_force(double dt);

	//�������е���
	void computeTotalForce(double dt);

	/**************������ĭ**************/


public:
	ShallowWaveSolver2Ptr _shallowWaveSolver;

private:
	FoamVortexDataPtr _foamVortexData;
};



#endif
