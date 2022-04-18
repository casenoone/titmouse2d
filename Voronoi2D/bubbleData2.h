#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"
#include <Eigen/Dense>
#include "../titmouse2d/src/Geometry/RegularPolygon.h"


class BubbleData2 : public ParticleSystemData2 {
public:
	BubbleData2() = default;

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


	public:
		Vector2D start;
		Vector2D end;
		Vector2D mid;
		Vector2D normal;
		Vector2D tangent;
		double gamma;
	};

	ArrayD& vorticities();


public:
	ArrayD particleRadius;

	RegularPolygonPtr panelSet;

	//��ȥ�������
	Eigen::VectorXd strength;

	//��ȥ�������
	Eigen::VectorXd slipStrength;

	//��ȥ�������
	Eigen::MatrixXd A;

	//��ȥ�������
	Eigen::MatrixXd B;

	Array<Vector2D> newParticles;

	//��ŷ����ȥ�������ӵ���Ϣ
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

	//tracer���ӵ��ٶȺ�λ��
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

private:
	//��ά����£������Ǳ���
	ArrayD _vorticities;
};

using BubbleDataPtr = std::shared_ptr<BubbleData2>;

inline ArrayD& BubbleData2::vorticities() {
	return _vorticities;
}