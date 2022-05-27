#pragma once

#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Array.hpp"
#include <Eigen/Dense>

class MassSpringData2 {

public:
	MassSpringData2() = default;

	MassSpringData2(const std::vector<Vector2D>& pos)
	{
		positions.set(pos);
		numberOfPoint = pos.size();
		velocities.reSize(numberOfPoint);
	}

	struct Edge {
		int i;
		int j;
	};



public:
	Array<Vector2D> velocities;

	Array<Vector2D> positions;

	//ϵͳ�е�Լ��
	Array<Edge> edges;

	//ϵͳ���ʵ������
	int numberOfPoint = 0;

	//����ϵͳ�ĸն�
	Eigen::MatrixXd CompliantMat;

	//Jacobin����
	Eigen::MatrixXd JacobinMat;

	//��ʱ�ľ�ֹ����
	double restLen = 0.06667;

	//��ʱ�ĸն�
	double stiff = 500;

	//����ϵ��
	double dampingCoeff = 20;
};

using MassSpringData2Ptr = std::shared_ptr<MassSpringData2>;
