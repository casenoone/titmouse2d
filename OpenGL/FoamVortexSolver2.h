#ifndef FOAMVORTEXSOLVER2_H
#define FOAMVORTEXSOLVER2_H


#include "FoamVortexData2.h"
#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/OtherMethod/SWE/ShallowWaveSolver2.h"
#include "../titmouse2d/src/boundingbox2.h"


#include <Eigen/Dense>


//坐标规定：
//x : pos.x
//y : height
//z : pos.y


class FoamVortexSolver : public ParticleSystemSolver2 {

public:
	FoamVortexSolver();

	//高度场与涡粒子耦合专用
	FoamVortexSolver(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin = Vector2D::zero());

	//时间积分
	virtual void timeIntegration(double timeIntervalInSeconds)override;

	//时步更新
	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	FoamVortexDataPtr& foamVortexData();

	//计算涡粒子对其他粒子的诱导速度
	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	//设置移动边界（使用siggraph2020的边界求解方法）
	void setMovingBoudnary(RegularPolygonPtr surfaces);

	void setMovingBoudnary(RecTanglePtr surfaces);

	//设置静态边界（使用siggraph2005的边界求解方法）
	void setStaticBoudnary(ExplicitSurface2Ptr surfaces);

	//发射追踪粒子
	void emitTracerParticles();



	//从移动边界上发射涡粒子以满足no-slip边界条件
	void emitParticlesFromPanels(double timeIntervalInSeconds);

	//发射vortex ring
	void emitVortexRing();

	//为高度场设置移动边界
	void setShallowWaveMovingBoundary(const Vector2D& center, const double r);

	//为高度场设置矩形移动边界
	void setShallowWaveMovingBoundary(const RecTanglePtr box);

	//计算流体与气泡的交互力
	Vector2D computeTwoWayForce(int index, double dt);

	//生成气泡对应的panelset
	void generatePanelSet(const Array<Vector2D>& pos, const Array<double>& radius);

	void generatePanelSet(const Array<Vector2D>& pos);


private:
	//求解tracer粒子
	void tracerParticlesSolve();

	//只要边界形状不变，边界矩阵就不会变
	//移动边界求解边界积分方程时使用的矩阵（SIGGRAPH2020）
	void constructMovingBoundaryMatrix();

	//静态边界求解边界积分方程时使用的矩阵（SIGGRAPH2005）
	void constructStaticBoundaryMatrix();

	//组装求解two-way问题时用到的矩阵
	void constructTwoWayBoundaryMatrix();

	//求解整个泡沫团上的vortex strength
	void all_bubble_vortexStrengthSolve(double dt);

	//组装移动边界对应的矩阵所用
	Vector2D computeUnitVelocityFromPanels(const Vector2D& pos, int index);

	//组装静态边界对应的矩阵所用
	Vector2D static_computeUnitVelocityFromPanels(int index, const Vector2D& midPoint);

	//组装twoway对应的矩阵所用
	Vector2D two_way_computeUnitVelocityFromPanels(int bubble_idx, int panelIdx, const Vector2D& midPoint);

	//求解静态边界满足不穿透边界条件下的vortex_strength
	void no_through_solve(double timeIntervalInSeconds);

	//静态边界在空间中某处的诱导速度
	Vector2D static_computeSingleVelocityFromPanels(int index);

	//求解追踪粒子的边界穿透问题
	void tarcerCollisionSolve(Vector2D& pos);

	//涡量耗散（这里可否提出一些稍微物理一点的耗散模型）
	void decayVorticity();

	//更新bubble_panelset的位置
	void update_bubble_panelset_pos(double dt);

private:
	/**************以下泡沫**************/

	void bubble_timeIntegration(double dt);

	//构造Compliant Matrix
	void constructCompliantMat();

	//构造Jacobin Matrix
	void constructJacobinMat();

	//构造系统中质点之间的约束
	void constructConstraint();

	//求导数（用于组装jacobin中的元素）
	Vector2D getDerivation(int phi_idx, int x_idx);

	//这个要修改成基于网格搜索的结构
	double computeConstraint(int idx);

	//构造解线性系统用到的constraint向量
	void construct_ConstraintVector(Eigen::VectorXd& vec);

	//构造求解线性系统用到的速度场向量组
	void construct_VelocityVector(Eigen::VectorXd& vec);

	//计算弹簧之间的阻尼
	Vector2D computeSpringDragForce(int idx);

	//计算所有的two-way力
	void compute_all_twoway_force(double dt);

	//计算所有的力
	void computeTotalForce(double dt);

	/**************以上泡沫**************/


public:
	ShallowWaveSolver2Ptr _shallowWaveSolver;

private:
	FoamVortexDataPtr _foamVortexData;
};



#endif
