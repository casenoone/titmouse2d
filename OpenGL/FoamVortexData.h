#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"

#include "../titmouse2d/src/Geometry/RegularPolygon.h"

#include "MovingFaceCenteredGrid2.h"

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

	ArrayD& gamma();

public:
	RegularPolygonPtr panelSet;

	//2021 paper �߽紦��
	//��ȥ�������
	Eigen::VectorXd slip_strength;

	//��ȥ�������
	Eigen::VectorXd no_throught_strength;

	//��ȥ�������
	Eigen::MatrixXd A;

	//��ȥ�������
	Eigen::MatrixXd slip_matrix;

	//���淢�����ӵ�λ��
	Array<Vector2D> newParticles;

	//tracer���ӵ��ٶȺ�λ��
	Array<Vector2D> tracePosition;
	Array<Vector2D> traceVelocity;

	//��ŷ����ȥ�������ӵ���Ϣ
	Array<Vector2D> vortexPosition;
	Array<Vector2D> vortexVelocity;

private:

	//��ά����£������Ǳ���
	ArrayD _gamma;


};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::gamma() {
	return _gamma;
}

#endif
