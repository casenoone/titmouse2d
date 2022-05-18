#pragma once

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/RegularPolygon.h"


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


class FoamVortexData : public ParticleSystemData2 {
public:
	FoamVortexData() = default;

	ArrayD& vorticities();

public:
	RegularPolygonPtr panelSet;

	//��ȥ�������
	Eigen::VectorXd no_through_strength;

	//��ȥ�������
	Eigen::VectorXd slip_Strength;

	//��ȥ�������
	Eigen::MatrixXd no_through_matrix;

	//��ȥ�������
	Eigen::MatrixXd slip_matrix;

	Array<Vector2D> newParticles;

	//tracer���ӵ��ٶȺ�λ��
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

	//��ŷ����ȥ�������ӵ���Ϣ
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

	//������ݶ�Ӧ��panelģ��
	Array<RegularPolygonPtr> bubble_panelset;

	//��������Ҫ�õ��ľ����gamma
	Eigen::MatrixXd bubble_slip_matrix;

	Array<Eigen::VectorXd> bubble_slip_strength;



private:

	//��ά����£������Ǳ���
	ArrayD _vorticities;


};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::vorticities() {
	return _vorticities;
}
