#include "SimpleLevelSetSolver2.h"
#include "../Numerical/Pde.hpp"
#include <iostream>
#include <array>

SimpleLevelSetSolver2::SimpleLevelSetSolver2() {

}

SimpleLevelSetSolver2::~SimpleLevelSetSolver2() {

}


double SimpleLevelSetSolver2::sign(const Array2D& input, int i, int j) const {
	auto h = input.dataSize().x;
	auto h2 = h * h;
	auto phi = input.lookAt(i, j);
	auto phi2 = phi * phi;
	return phi / (sqrt(phi2 + h2));
}

//不知道如何理解这里的Cfl值
//与时步有何关系？
//这个函数的算法原理没搞清楚
double SimpleLevelSetSolver2::pseudoTimeStep(const Array2D& input,
	const Vector2D& gridSpacing) {
	double h = gridSpacing.x;

	auto size = input.dataSize();

	double maxS = -std::numeric_limits<double>::max();
	double dtau = _maxCfl * h;

	for (int i = 0; i < size.x; ++i) {
		for (int j = 0; j < size.y; ++j) {
			double s = sign(input, i, j);
			maxS = std::max(s, maxS);
		}
	}

	while (dtau * maxS / h > _maxCfl) {
		dtau *= 0.5;
	}

	return dtau;
}

int SimpleLevelSetSolver2::distanceToNumberOfIterations(double distance, double dtau) {
	return static_cast<int>(std::ceil(distance / dtau));
}


void SimpleLevelSetSolver2::getDerivatives(const Array2D& input, Vector2D gridSpacing,
	int i, int j, std::array<double, 2>* dx, std::array<double, 2>* dy) const {

	double D0[3];
	auto size = input.dataSize();

	int im1 = (i < 1) ? 0 : i - 1;
	int ip1 = std::min(i + 1, size.x - 1);
	int jm1 = (j < 1) ? 0 : j - 1;
	int jp1 = std::min(j + 1, size.y - 1);

	D0[0] = input.lookAt(im1, j);
	D0[1] = input.lookAt(i, j);
	D0[2] = input.lookAt(ip1, j);
	*dx = upwind1(D0, double(gridSpacing.x));

	D0[0] = input.lookAt(i, jm1);
	D0[1] = input.lookAt(i, j);
	D0[2] = input.lookAt(i, jp1);
	*dy = upwind1(D0, double(gridSpacing.y));

}


//这里有一个语法上的问题
//比如说auto arr = Array2对象时
//此时并不会调用Array2里的等于号重载

//那么auto grid = VertexCentered时，
//grid类里面的arr和上面是一样的过程么？会调用重载运算符么？
void SimpleLevelSetSolver2::reinitialize(const VertexCenteredScalarGrid2& inputSdf, double maxDistance,
	VertexCenteredScalarGrid2Ptr& outputSdf) {

	auto size = inputSdf.dataSize();
	auto gridSpacing = inputSdf.gridSpacing();

	Array2D input;
	input.reSize(size.x, size.y);

	//我擦，这里应该不会出错吧？
	//应该不会，这里inputSdf是const类型，因此调用的是const版本的datas()
	//const类型的也会调用重载运算符，没有问题！
	input = inputSdf.datas();

	double dtau = pseudoTimeStep(
		input, gridSpacing);
	int numberOfIterations
		= distanceToNumberOfIterations(maxDistance, dtau);

	for (int n = 0; n < numberOfIterations; ++n) {
		//里面这层循环可以并行
		for (int i = 0; i < size.x; ++i) {
			for (int j = 0; j < size.y; ++j) {
				double s = sign(input, i, j);

				std::array<double, 2> dx, dy;
				//计算得在x、y方向上sdf的梯度
				getDerivatives(input, gridSpacing, i, j, &dx, &dy);

				//显式欧拉时间积分
				(*outputSdf)(i, j) = input.lookAt(i, j)
					- dtau * std::max(s, 0.0)
					* (std::sqrt(square(std::max(dx[0], 0.0))
						+ square(std::min(dx[1], 0.0))
						+ square(std::max(dy[0], 0.0))
						+ square(std::min(dy[1], 0.0))) - 1.0)
					- dtau * std::min(s, 0.0)
					* (std::sqrt(square(std::min(dx[0], 0.0))
						+ square(std::max(dx[1], 0.0))
						+ square(std::min(dy[0], 0.0))
						+ square(std::max(dy[1], 0.0))) - 1.0);
			}
		}
		std::swap(input, outputSdf->datas());
	}

	outputSdf->datas() = input;
}