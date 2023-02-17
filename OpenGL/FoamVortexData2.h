#ifndef FOAMVORTEXDATA_H
#define FOAMVORTEXDATA_H

#include <Eigen/Dense>
#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"
#include "../titmouse2d/src/Geometry/RegularPolygon.h"
#include "../titmouse2d/src/Geometry/RecTangle.h"
#include "MovingFaceCenteredGrid2.h"
#include <Eigen/Sparse>

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

	struct Edge {
		int i;
		int j;
	};

	/**************������ĭ**************/

	//ϵͳ�е�Լ��
	Array<Edge> edges;

	//����ϵͳ�ĸն�
	Eigen::SparseMatrix<double> CompliantMat;

	//Jacobin����
	Eigen::SparseMatrix<double> JacobinMat;

	//��ʱ�ľ�ֹ����
	double restLen = 0.0001;

	Array<double> restLens;
	Array<double> radiuss;

	//��ʱ�ĸն�
	//double stiff = 8000000;
	double stiff = 100000;
	//����ϵ��
	double dampingCoeff = 30;

	//��ʱ��bubble�ߴ�
	double radius = 0.1;

	/**************������ĭ**************/


	//RegularPolygonPtr panelSet;
	RecTanglePtr panelSet;
	std::vector<Panel> staticBoundary;

	//2021 paper �߽紦��
	//��ȥ�������
	Eigen::VectorXd slip_strength;

	//��ȥ�������
	Eigen::VectorXd no_through_strength;

	//��ȥ�������
	Eigen::MatrixXd no_through_matrix;

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


	//������ݶ�Ӧ��panelģ��
	Array<RegularPolygonPtr> bubble_panelset;

	//��������Ҫ�õ��ľ����gamma
	Eigen::MatrixXd bubble_slip_matrix;

	Array<Eigen::VectorXd> bubble_slip_strength;

	//�����ʼ����
	ArrayD initVorticity;

private:

	//��ά����£������Ǳ���
	ArrayD _gamma;




};

using FoamVortexDataPtr = std::shared_ptr<FoamVortexData>;

inline ArrayD& FoamVortexData::gamma() {
	return _gamma;
}

#endif
