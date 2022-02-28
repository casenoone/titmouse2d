#include "ShallowWaveSolver2.h"
#include "../../LinearSystem/ConjugateGradientSolver.hpp"

const double shallow_H = 1.0;

//控制流体的粘性
const double shallow_beta = 1.0;


//假设标签的值为1时是被固体占据
void ShallowWaveSolver2::setFluidCellNum() {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto markers = data->markers;
	auto solveSystemMarker = data->solveSystemMarker;

	int num = 0;

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (markers(i, j) == 1) {
				++num;
				solveSystemMarker(i, j) = num;
			}
		}
	}
}


//注意，每一个循环解算出来的新值，都赋给old_h
//最后都求解完再将二者交换
void ShallowWaveSolver2::pressureSolve(double timeIntervalInSeconds) {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto old_h = data->old_height->datas();
	auto h = data->height->datas();

	auto t2 = timeIntervalInSeconds * timeIntervalInSeconds;
	auto x2 = data->gridSpacing().x * gridSpacing().x;
	auto alpha = t2 * shallow_H * _gravity.y / x2;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {

			auto term1 = h(i, j) + shallow_beta * (h(i, j) - old_h(i, j));

			int up = 1, down = 1, left = 1, right = 1;
			double current = -4;
			double sum = 0.0;
			//如果上边界是固体
			if (j + 1 == res.y) {
				up = 0;
				--current;
			}

			//如果下边界是固体
			if (j - 1 == 0) {
				down = 0;
				--current;
			}

			//如果左边是固体
			if (i - 1 == 0) {
				left = 0;
				--current;
			}

			//如果右边是固体
			if (i + 1 == res.x) {
				right = 0;
				--current;
			}

			sum += current * h(i, j);
			if (up != 0)sum += h(i, j + 1);
			if (down != 0)sum += h(i, j - 1);
			if (left != 0)sum += h(i - 1, j);
			if (right != 0)sum += h(i + 1, j);

			old_h(i, j) = term1 + sum * alpha;
		}
	}
}



void ShallowWaveSolver2::couplingSolve(double timeIntervalInSeconds) {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto old_h = data->old_height->datas();
	auto h = data->height->datas();
	auto marker = data->markers;
	auto cellNum = data->solveSystemMarker;
	auto v = data->ghostH;
	v.reSize(res.x, res.y, 0.0);

	int systemSize = 0;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (marker(i, j) == 1)systemSize++;
		}
	}

	auto t2 = timeIntervalInSeconds * timeIntervalInSeconds;
	auto x2 = data->gridSpacing().x * gridSpacing().x;
	auto alpha = t2 * shallow_H * _gravity.y / x2;
	VectorN<double> x(systemSize);
	VectorN<double> b(systemSize);
	SparseMatrix<double> A(systemSize, systemSize);

	int row = 0;

	//暂时假设固体障碍物的高度是0.1
	double e = 0.1;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			int coeff = -4;
			//如果当前格子是耦合格子
			if (marker(i, j) == 1) {
				if (i + 1 < res.x) {
					if (marker.lookAt(i + 1, j) == FLUID) {
						A.insert(row, cellNum(i + 1, j), -1);
					}
				}

				else {
					++coeff;
				}

				if (i - 1 >= 0) {
					if (marker.lookAt(i - 1, j) == FLUID) {
						A.insert(row, cellNum(i - 1, j), -1);
					}
				}
				else {
					++coeff;
				}


				if (j + 1 < res.y) {
					if (marker.lookAt(i, j + 1) == FLUID) {
						A.insert(row, cellNum(i, j + 1), -1);
					}
				}
				else {
					++coeff;
				}

				if (j - 1 >= 0) {
					if (marker.lookAt(i, j - 1) == FLUID) {
						A.insert(row, cellNum(i, j - 1), -1);
					}
				}
				else {
					++coeff;
				}

				A.insert(row, cellNum(i, j), -coeff);
				b[row] = (old_h(i, j) - h(i, j) + e) / alpha;
				++row;
			}

		}

	}

	A.build();
	ConjugateGradientSolver<double> cgSolver;
	cgSolver.compute(A, x, b);

	//装载虚拟水
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (cellNum(i, j) != -1) {
				v(i, j) = b(cellNum(i, j));
			}
		}
	}

}

void applyGhostVolumn() {

}