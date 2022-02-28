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

//��֪�������������Cflֵ
//��ʱ���кι�ϵ��
//����������㷨ԭ��û�����
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


//������һ���﷨�ϵ�����
//����˵auto arr = Array2����ʱ
//��ʱ���������Array2��ĵ��ں�����

//��ôauto grid = VertexCenteredʱ��
//grid�������arr��������һ���Ĺ���ô����������������ô��
void SimpleLevelSetSolver2::reinitialize(const VertexCenteredScalarGrid2& inputSdf, double maxDistance,
	VertexCenteredScalarGrid2Ptr& outputSdf) {

	auto size = inputSdf.dataSize();
	auto gridSpacing = inputSdf.gridSpacing();

	Array2D input;
	input.reSize(size.x, size.y);

	//�Ҳ�������Ӧ�ò������ɣ�
	//Ӧ�ò��ᣬ����inputSdf��const���ͣ���˵��õ���const�汾��datas()
	//const���͵�Ҳ����������������û�����⣡
	input = inputSdf.datas();

	double dtau = pseudoTimeStep(
		input, gridSpacing);
	int numberOfIterations
		= distanceToNumberOfIterations(maxDistance, dtau);

	for (int n = 0; n < numberOfIterations; ++n) {
		//�������ѭ�����Բ���
		for (int i = 0; i < size.x; ++i) {
			for (int j = 0; j < size.y; ++j) {
				double s = sign(input, i, j);

				std::array<double, 2> dx, dy;
				//�������x��y������sdf���ݶ�
				getDerivatives(input, gridSpacing, i, j, &dx, &dy);

				//��ʽŷ��ʱ�����
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