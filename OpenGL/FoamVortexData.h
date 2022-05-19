#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/RegularPolygon.h"

#include "MovingFaceCenteredGrid2.h"



class FoamVortexData : public ParticleSystemData2 {
public:
	FoamVortexData() = default;

	ArrayD& gamma();

public:

	class Panel {
	public:
		Panel() :
			start(Vector2D::zero()),
			end(Vector2D::zero()),
			mid(Vector2D::zero()),
			normal(Vector2D::zero()),
			tangent(Vector2D::zero()),
			gamma(0.0)
		{}

		Panel(const Vector2D& start_,
			const Vector2D& end_,
			const Vector2D& mid_,
			const Vector2D& normal_) :
			start(start_),
			end(end_),
			mid(mid_),
			normal(normal_)
		{}

	public:
		Vector2D start;
		Vector2D end;
		Vector2D mid;
		Vector2D normal;
		Vector2D tangent;
		double gamma;
	};

	/**************以下泡沫**************/

	//调节bubble之间排斥力的系数
	double kr = 18;

	//调节bubble之间吸引力的系数
	double ka = 15;

	//调节空气阻力
	double kair = 7;

	ArrayD particleRadius;

	/**************以上泡沫**************/


	RegularPolygonPtr panelSet;
	std::vector<Panel> staticBoundary;

	//2021 paper 边界处理
	//消去切向分量
	Eigen::VectorXd slip_strength;

	//消去法向分量
	Eigen::VectorXd no_through_strength;

	//消去法向分量
	Eigen::MatrixXd no_through_matrix;

	//消去切向分量
	Eigen::MatrixXd slip_matrix;

	//保存发射粒子的位置
	Array<Vector2D> newParticles;

	//tracer粒子的速度和位置
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

	//存放发射出去的涡粒子的信息
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

	//two-way bubble

	//存放气泡对应的panel模型
	Array<RegularPolygonPtr> bubble_panelset;

	//气泡所需要用到的矩阵和gamma
	Eigen::MatrixXd bubble_slip_matrix;

	Array<Eigen::VectorXd> bubble_slip_strength;

private:

	//二维情况下，涡量是标量
	ArrayD _gamma;


};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::gamma() {
	return _gamma;
}

#endif
