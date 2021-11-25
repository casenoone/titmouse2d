#include "SimplePressureSolver2.h"
#include "../LinearSystem/ConjugateGradientSolver.hpp"
#include "../LinearSystem/SteepestDescentSolver.hpp"
#include "../ConstVar.h"


#include "../LinearSystem/GaussSeidelSolver.hpp"
SimplePressureSolver2::SimplePressureSolver2() {

}

SimplePressureSolver2::~SimplePressureSolver2() {

}


void SimplePressureSolver2::solve(FaceCenteredGrid2Ptr flow, const Array2Ptr<int>& markers) {
	int size = fluidCellSize(markers);
	VectorNPtr<double> x(size);
	constructMatrix(flow, markers, x, size);
	applyGradientandUpdateVel(flow, markers, x); 

}

//�ǳ��ʺ�OpenMP����ѽ��
int SimplePressureSolver2::fluidCellSize(const Array2Ptr<int>& markers) {
	int systemSize = 0;

	auto markersSize = markers.dataSize();
	for (size_t i = 0; i < markersSize.x; ++i) {
		for (size_t j = 0; j < markersSize.y; ++j) {
			if (markers.lookAt(i,j) == FLUID) {
				systemSize++;
			}
		}
	}
	return systemSize;
}

void SimplePressureSolver2::constructMatrix(FaceCenteredGrid2Ptr& flow, 
	const Array2Ptr<int>& markers,
	VectorNPtr<double> &x,int systemSize) {

	auto& solveSystemMarker = flow->solveSystemMarker;
	auto& u = flow->uDatas();
	auto& v = flow->vDatas();

	auto resolution = flow->resolution();

	auto H = flow->gridSpacing().x;
	auto invH = 1 / H;
	auto invH2 = invH * invH;

	VectorNPtr<double> b(systemSize);
	SparseMatrixPtr<double> A(systemSize, systemSize);

	int row = 0;


	for (int j = 0; j < resolution.y; ++j) {
		for (int i = 0; i < resolution.x; ++i) {
			int coeff = -4;

			//����Ԫ������
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
					//cout << i << endl;
				}

				if (j <= 0 || j >= flow->vSize().y - 1) {
					v(i, j) = 0;
				}
				
				b[row] = -(u(i + 1, j) - u(i, j) + v(i, j + 1) - v(i, j));
				++row;
			}
			
		}
	}
	
	//ǧ��Ҫ����build����
	A.build();


	ConjugateGradientSolver<double> cgSolver;
	GaussSeidelSolver<double> gauSolver;
	SteepestDescentSolver<double> steepSolver;
	
	//����ϵͳ�����������Ż�
	cgSolver.compute(A, x, b);
	//gauSolver.compute(A, x, b);
	//steepSolver.compute(A, x, b);
	
	//x *= 1000000;
	
}

void SimplePressureSolver2::applyGradientandUpdateVel(FaceCenteredGrid2Ptr& flow, 
	const Array2Ptr<int>& markers,
	VectorNPtr<double> &x) {


	auto& u = flow->uDatas();
	auto& v = flow->vDatas();
	auto H = flow->gridSpacing().x;

	auto invH = 1.0 / H;

	auto& solveSystemMarker = flow->solveSystemMarker;

	auto sizeU = flow->uSize();
	for (size_t j = 0; j < sizeU.y; ++j) {
		for (size_t i = 0; i < sizeU.x; ++i) {
			//˵���ڱ߽���
			if (i <= 0 || i >= sizeU.x - 1) {
				u(i, j) = 0;
			}
			else {
				//������Ҷ�������
				if (markers.lookAt(i - 1, j) == FLUID && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					auto col0 = solveSystemMarker(i - 1, j);
					u(i, j) = u(i, j) - (x[col1] - x[col0]) * invH;
				}
				//�����������壬�ұ�������
				if (markers.lookAt(i - 1, j) == AIR && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					u(i, j) = u(i, j) - x[col1] * invH;
				}
				//�����������壬�ұ�������
				if (markers.lookAt(i - 1, j) == FLUID && markers.lookAt(i, j) == AIR) {
					auto col0 = solveSystemMarker(i - 1, j);
					u(i, j) = u(i, j) - (0 - x[col0]) * invH;
				}


			}
		}
	}


	auto sizeV = flow->vSize();
	for (size_t j = 0; j < sizeV.y; ++j) {
		for (size_t i = 0; i < sizeV.x; ++i) {
			//˵���ڱ߽���
			if (j <= 0 || j >= sizeV.y - 1) {
				v(i, j) = 0;
			}
			else {
				//������¶�������
				if (markers.lookAt(i, j - 1) == FLUID && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					auto col0 = solveSystemMarker(i, j - 1);
					v(i, j) = v(i, j) - (x[col1] - x[col0]) * invH;
				}
				//����������壬�ϱ�������

				if (markers.lookAt(i, j - 1) == AIR && markers.lookAt(i, j) == FLUID) {
					auto col1 = solveSystemMarker(i, j);
					v(i, j) = v(i, j) - x[col1] * invH;
				}
				//����±������壬�ϱ�������

				if (markers.lookAt(i, j - 1) == FLUID && markers.lookAt(i, j) == AIR) {
					auto col0 = solveSystemMarker(i, j - 1);
					v(i, j) = v(i, j) - (0 - x[col0]) * invH;
				}


			}
		}
	}

}