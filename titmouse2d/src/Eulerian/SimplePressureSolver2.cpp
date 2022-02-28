#include "SimplePressureSolver2.h"
#include "../LinearSystem/ConjugateGradientSolver.hpp"
#include "../LinearSystem/SteepestDescentSolver.hpp"
#include "../LinearSystem/JacobiSolver.hpp"
#include "../LinearSystem/SORsolver.hpp"
#include "../LinearSystem/RelaxedJacobiSolver.hpp"

#include "../ConstVar.h"


#include "../LinearSystem/GaussSeidelSolver.hpp"

#include<omp.h>
SimplePressureSolver2::SimplePressureSolver2() {

}

SimplePressureSolver2::~SimplePressureSolver2() {

}


void SimplePressureSolver2::solve(FaceCenteredGrid2Ptr flow, const Array2I& markers) {
	int size = fluidCellSize(markers);
	VectorN<double> x(size);

	constructMatrix(flow, markers, x, size);
	applyGradientandUpdateVel(flow, markers, x);
}

int SimplePressureSolver2::fluidCellSize(const Array2I& markers) {
	int systemSize = 0;

	auto markersSize = markers.dataSize();
	omp_set_num_threads(23);
#pragma omp parallel for reduction(+:systemSize)
	for (int i = 0; i < markersSize.x; ++i) {
		for (int j = 0; j < markersSize.y; ++j) {
			if (markers.lookAt(i, j) == FLUID) {
				systemSize += 1;
			}
		}
	}


	return systemSize;
}


void SimplePressureSolver2::constructMatrix(FaceCenteredGrid2Ptr& flow,
	const Array2I& markers,
	VectorN<double>& x, int systemSize) {

	auto& solveSystemMarker = flow->solveSystemMarker;
	auto& u = flow->uDatas();
	auto& v = flow->vDatas();

	auto resolution = flow->resolution();

	auto H = flow->gridSpacing().x;
	auto invH = 1 / H;
	auto invH2 = invH * invH;

	VectorN<double> b(systemSize);

	SparseMatrix<double> A(systemSize, systemSize);

	int row = 0;


	for (int j = 0; j < resolution.y; ++j) {
		for (int i = 0; i < resolution.x; ++i) {
			int coeff = -4;
			//网格单元是流体
			if (markers.lookAt(i, j) == FLUID) {

				if (i + 1 < resolution.x) {
					if (markers.lookAt(i + 1, j) == FLUID) {
						A.insert(row, solveSystemMarker(i + 1, j), -invH);
					}
				}

				else {
					++coeff;
				}

				if (i - 1 >= 0) {
					if (markers.lookAt(i - 1, j) == FLUID) {
						A.insert(row, solveSystemMarker(i - 1, j), -invH);
					}
				}
				else {
					++coeff;
				}


				if (j + 1 < resolution.y) {
					if (markers.lookAt(i, j + 1) == FLUID) {
						A.insert(row, solveSystemMarker(i, j + 1), -invH);
					}
				}
				else {
					++coeff;
				}

				if (j - 1 >= 0) {
					if (markers.lookAt(i, j - 1) == FLUID) {
						A.insert(row, solveSystemMarker(i, j - 1), -invH);
					}
				}
				else {
					++coeff;
				}

				A.insert(row, solveSystemMarker(i, j), -coeff * invH);
				if (i <= 0 || i >= flow->uSize().x - 1) {
					u(i, j) = 0;

				}

				if (j <= 0 || j >= flow->vSize().y - 1) {
					v(i, j) = 0;
				}
				b[row] = -(u(i + 1, j) - u(i, j) + v(i, j + 1) - v(i, j));
				++row;

			}
		}
	}

	//千万不要忘记build（）
	A.build();


	ConjugateGradientSolver<double> cgSolver;
	//GaussSeidelSolver<double> gauSolver;
	//SteepestDescentSolver<double> steepSolver;
	//JacobiSolver<double> JacobiSolver;
	//RelaxedJacobiSolver<double> rjSolver(0.5);
	//SORSolver<double> sorSolver(1.67);

	////线性系统求解可作并行优化
	cgSolver.compute(A, x, b);
	////gauSolver.compute(A, x, b);
	////steepSolver.compute(A, x, b);
	////JacobiSolver.compute(A, x, b);
	////rjSolver.compute(A, x, b);
	//sorSolver.compute(A, x, b);

}

void SimplePressureSolver2::applyGradientandUpdateVel(FaceCenteredGrid2Ptr& flow,
	const Array2I& markers,
	VectorN<double>& x) {


	auto& u = flow->uDatas();
	auto& v = flow->vDatas();
	auto H = flow->gridSpacing().x;

	auto invH = 1.0 / H;

	auto& solveSystemMarker = flow->solveSystemMarker;

	auto sizeU = flow->uSize();

	omp_set_num_threads(23);
#pragma omp parallel for 
	for (int j = 0; j < sizeU.y; ++j) {
		for (int i = 0; i < sizeU.x; ++i) {
			//说明在边界上
			if (i <= 0 || i >= sizeU.x - 1) {
				u(i, j) = 0;
			}
			else {
				//如果左右都是流体
				if (markers.lookAt(i - 1, j) == FLUID && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					auto col0 = solveSystemMarker(i - 1, j);
					u(i, j) -= (x[col1] - x[col0]) * invH;
				}
				//如果左边是气体，右边是流体
				if (markers.lookAt(i - 1, j) == AIR && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					u(i, j) -= x[col1] * invH;
				}
				//如果左边是流体，右边是气体
				if (markers.lookAt(i - 1, j) == FLUID && markers.lookAt(i, j) == AIR) {
					auto col0 = solveSystemMarker(i - 1, j);
					u(i, j) += (x[col0]) * invH;
				}


			}
		}
	}


	auto sizeV = flow->vSize();
	omp_set_num_threads(23);
#pragma omp parallel for 
	for (int j = 0; j < sizeV.y; ++j) {
		for (int i = 0; i < sizeV.x; ++i) {
			//说明在边界上
			if (j <= 0 || j >= sizeV.y - 1) {
				v(i, j) = 0;
			}
			else {
				//如果上下都是流体
				if (markers.lookAt(i, j - 1) == FLUID && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					auto col0 = solveSystemMarker(i, j - 1);
					v(i, j) -= (x[col1] - x[col0]) * invH;
				}
				//如果下是气体，上边是流体

				if (markers.lookAt(i, j - 1) == AIR && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					v(i, j) -= x[col1] * invH;
				}
				//如果下边是流体，上边是气体

				if (markers.lookAt(i, j - 1) == FLUID && markers.lookAt(i, j) == AIR) {
					auto col0 = solveSystemMarker(i, j - 1);
					v(i, j) += (x[col0]) * invH;
				}


			}
		}
	}

}