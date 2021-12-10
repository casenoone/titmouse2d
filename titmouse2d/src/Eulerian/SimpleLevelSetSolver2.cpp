#include "SimpleLevelSetSolver2.h"

#include <iostream>
#include <array>

SimpleLevelSetSolver2::SimpleLevelSetSolver2() {

}

SimpleLevelSetSolver2::~SimpleLevelSetSolver2() {

}


double SimpleLevelSetSolver2::sign(const VertexCenteredScalarGrid2& inputSdf, int i, int j) const {
	auto h = inputSdf.gridSpacing().x;
	auto h2 = h * h;
	auto phi = inputSdf.lookAt(i, j);
	auto phi2 = phi * phi;
	return phi / (sqrt(phi2 + h2));
}


void SimpleLevelSetSolver2::getDerivatives(const VertexCenteredScalarGrid2& inputSdf, Vector2<size_t> gridSpacing,
	size_t i, size_t j, std::array<double, 2>* arr) {

	/*double D0[3];
	auto size = inputSdf.dataSize();

	size_t im1 = (i < 1) ? 0 : i - 1;
	size_t ip1 = std::min(i + 1, size.x - 1);
	size_t jm1 = (j < 1) ? 0 : j - 1;
	size_t jp1 = std::min(j + 1, size.y - 1);

	D0[0] = inputSdf.lookAt(im1, j);
	D0[1] = inputSdf.lookAt(i, j);
	D0[2] = inputSdf.lookAt(ip1, j);
	*dx = upwind1(D0, gridSpacing.x);

	D0[0] = grid(i, jm1);
	D0[1] = grid(i, j);
	D0[2] = grid(i, jp1);
	*dy = upwind1(D0, gridSpacing.y);*/

}


void SimpleLevelSetSolver2::reinitialize(const VertexCenteredScalarGrid2& inputSdf, double maxDistance,
	VertexCenteredScalarGrid2Ptr& outputSdf) {

	auto size = inputSdf.dataSize();
	auto gridSpacing = inputSdf.gridSpacing();
	
	(*outputSdf) = inputSdf;

	Array2Ptr<double> temp;
	temp.reSize(size.x, size.y);

	for (int n = 0; n < numberOfIterations; ++n) {
		//里面这层循环可以并行
		for (int i = 0; i < size.x; ++i) {
			for (int j = 0; j < size.y; ++j) {
				double s = sign(inputSdf, i, j);

				std::array<double, 2> dx, dy;


			}
		}

	}
}