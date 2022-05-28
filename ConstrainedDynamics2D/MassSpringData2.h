#pragma once

#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Array.hpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>
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

	//系统中的约束
	Array<Edge> edges;

	//系统中质点的数量
	int numberOfPoint = 0;

	//控制系统的刚度
	Eigen::SparseMatrix<double> CompliantMat;

	//Jacobin矩阵
	Eigen::SparseMatrix<double> JacobinMat;

	//临时的静止长度
	double restLen = 0.06667;

	//临时的刚度
	double stiff = 80000;

	//阻尼系数
	double dampingCoeff = 20;
};

using MassSpringData2Ptr = std::shared_ptr<MassSpringData2>;
