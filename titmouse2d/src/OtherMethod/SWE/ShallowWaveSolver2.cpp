#include "ShallowWaveSolver2.h"
#include "../../LinearSystem/ConjugateGradientSolver.hpp"
#include "../../Vector3.hpp"
#include "../../mesh/objout.hpp"
#include "../../mesh/Plyout.h"

const double shallow_H = 1.0;

//控制流体的粘性
const double shallow_beta = 0.9;

//控制耦合稳定性
const double shallow_gama = 0.4;

const double shallow_e = 0.035;

//假设标签的值为1时是被固体占据
void ShallowWaveSolver2::setCouplingCellNum() {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto markers = data->markers;
	auto solveSystemMarker = data->solveSystemMarker;

	solveSystemMarker.reSize(res.x, res.y, -1);

	int num = 0;

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (markers(i, j) == 1) {
				solveSystemMarker(i, j) = num;
				++num;
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
	auto alpha = t2 * shallow_H * 9.8 / x2;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {

			auto term1 = h(i, j) + shallow_beta * (h(i, j) - old_h(i, j));

			int up = 1, down = 1, left = 1, right = 1;
			double current = -4;
			double sum = 0.0;
			//如果上边界是固体
			if (j + 1 > res.y - 1) {
				up = 0;
				++current;
			}

			//如果下边界是固体
			if (j - 1 < 0) {
				down = 0;
				++current;
			}

			//如果左边是固体
			if (i - 1 < 0) {
				left = 0;
				++current;
			}

			//如果右边是固体
			if (i + 1 > res.x - 1) {
				right = 0;
				++current;
			}

			sum = current * h(i, j);
			if (up != 0) { sum += h(i, j + 1); }
			if (down != 0) { sum += h(i, j - 1); }
			if (left != 0) { sum += h(i - 1, j); }
			if (right != 0) { sum += h(i + 1, j); }
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
	auto alpha = t2 * shallow_H * 9.8 / x2;
	VectorN<double> x(systemSize);
	VectorN<double> b(systemSize);
	SparseMatrix<double> A(systemSize, systemSize);

	int row = 0;

	//暂时假设固体障碍物的高度是0.1

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			int coeff = -4;
			//如果当前格子是耦合格子
			if (marker(i, j) == 1) {
				if (i + 1 < res.x) {
					if (marker.lookAt(i + 1, j) == 1) {
						A.insert(row, static_cast<int>(cellNum(i + 1, j)), -1);
					}
				}

				else {
					++coeff;
				}

				if (i - 1 >= 0) {
					if (marker.lookAt(i - 1, j) == 1) {
						A.insert(row, static_cast<int>(cellNum(i - 1, j)), -1);
					}
				}
				else {
					++coeff;
				}


				if (j + 1 < res.y) {
					if (marker.lookAt(i, j + 1) == 1) {
						A.insert(row, static_cast<int>(cellNum(i, j + 1)), -1);
					}
				}
				else {
					++coeff;
				}

				if (j - 1 >= 0) {
					if (marker.lookAt(i, j - 1) == 1) {
						A.insert(row, static_cast<int>(cellNum(i, j - 1)), -1);
					}
				}
				else {
					++coeff;
				}

				A.insert(row, cellNum(i, j), -coeff);
				b[row] = (old_h(i, j) - h(i, j) + shallow_e) / alpha;
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
			if (static_cast<int>(cellNum(i, j)) != -1) {
				v(i, j) = x(static_cast<int>(cellNum(i, j)));
			}
		}
	}

}

void ShallowWaveSolver2::applyGhostVolumn(double timeIntervalInSeconds) {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto marker = data->markers;
	auto old_h = data->old_height->datas();
	auto h = data->height->datas();
	auto v = data->ghostH;
	auto t2 = timeIntervalInSeconds * timeIntervalInSeconds;
	auto x2 = data->gridSpacing().x * gridSpacing().x;
	auto alpha = t2 * shallow_H * 9.8 / x2;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			int up = 1, down = 1, left = 1, right = 1;
			double current = -4;
			double sum = 0.0;
			//如果上边界是固体
			if (j + 1 == res.y) {
				up = 0;
				++current;
			}

			//如果下边界是固体
			if (j - 1 < 0) {
				down = 0;
				++current;
			}

			//如果左边是固体
			if (i - 1 < 0) {
				left = 0;
				++current;
			}

			//如果右边是固体
			if (i + 1 == res.x) {
				right = 0;
				++current;
			}

			sum += current * v(i, j);
			if (up != 0)sum += v(i, j + 1);
			if (down != 0)sum += v(i, j - 1);
			if (left != 0)sum += v(i - 1, j);
			if (right != 0)sum += v(i + 1, j);

			old_h(i, j) = old_h(i, j) + sum * alpha * shallow_gama;

		}
	}

	old_h.swap(h);
}


void ShallowWaveSolver2::setMarkers(const Vector2D& lower, const Vector2D& upper) {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto marker = data->markers;
	auto h = data->height;

	marker.reSize(res.x, res.y, 0);

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			auto posFunc = h->dataPosition();
			auto pos = posFunc(i, j);
			if (pos.x >= lower.x && pos.x <= upper.x) {
				if (pos.y >= lower.y && pos.y <= upper.y) {
					marker(i, j) = 1;
				}
			}
		}
	}
}


void ShallowWaveSolver2::setSphereMarkers(const Vector2D& center, const double r) {
	auto data = _shallowWaveData;
	auto res = data->resolution();
	auto marker = data->markers;
	auto h = data->height;

	marker.reSize(res.x, res.y, 0);

	auto r2 = r * r;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			auto posFunc = h->dataPosition();
			auto pos = posFunc(i, j);
			if (pos.disSquare(center) <= r2) {
				marker(i, j) = 1;
			}
		}
	}
}



void ShallowWaveSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	setCouplingCellNum();
	pressureSolve(timeIntervalInSeconds);
	couplingSolve(timeIntervalInSeconds);
	applyGhostVolumn(timeIntervalInSeconds);
}

//在这里生成.obj文件
//坐标系（0，2）x（0，2）
int ShallowWaveSolver2::getWaterSurface(float* mesh) {
	auto data = _shallowWaveData;
	auto h = data->height;
	double temp_k = 30;
	auto res = data->resolution();
	int len = res.x * res.y;
	int k = 0;
	int triNum = 0;
	double temppp = -5;
	auto posFunc = data->height->dataPosition();


	static int fileNum = 1;
	std::string	name = std::to_string(fileNum);
	fileNum++;
	std::string path = "E:\\zhangjian\\solve_data\\ply_test\\";
	Plyout writer(path, name, res.x * res.y);


	//存储索引
	std::vector<Vector3I> tempArray;
	//用以生成索引
	int indexNum = 1;
	for (int j = res.y - 1; j > 0; j--) {
		for (int i = 0; i < res.x - 1; ++i) {
			auto pos1 = posFunc(i, j);
			auto tempX1 = (pos1.x - 1) * temp_k;
			auto tempY1 = (h->lookAt(i, j) - 1) * temp_k;
			auto tempZ1 = (pos1.y - 1) * temp_k;
			Vector3D p1(tempX1, tempY1, tempZ1);
			//writer.write_in_ply(pos1.x, h->lookAt(i, j), pos1.y);

			auto pos2 = posFunc(i + 1, j);
			auto tempX2 = (pos2.x - 1) * temp_k;
			auto tempY2 = (h->lookAt(i + 1, j) - 1) * temp_k;
			auto tempZ2 = (pos2.y - 1) * temp_k;
			Vector3D p2(tempX2, tempY2, tempZ2);

			auto pos3 = posFunc(i, j - 1);
			auto tempX3 = (pos3.x - 1) * temp_k;
			auto tempY3 = (h->lookAt(i, j - 1) - 1) * temp_k;
			auto tempZ3 = (pos3.y - 1) * temp_k;
			Vector3D p3(tempX3, tempY3, tempZ3);

			auto n = (p2 - p1).cross(p3 - p1).getNormalize();
			mesh[k] = static_cast<float>(p1.x);
			mesh[k + 1] = static_cast<float>(p1.y);
			mesh[k + 2] = static_cast<float>(p1.z);
			mesh[k + 3] = static_cast<float>(n.x);
			mesh[k + 4] = static_cast<float>(n.y);
			mesh[k + 5] = static_cast<float>(n.z);
			mesh[k + 6] = static_cast<float>(p2.x);
			mesh[k + 7] = static_cast<float>(p2.y);
			mesh[k + 8] = static_cast<float>(p2.z);
			mesh[k + 9] = static_cast<float>(n.x);
			mesh[k + 10] = static_cast<float>(n.y);
			mesh[k + 11] = static_cast<float>(n.z);
			mesh[k + 12] = static_cast<float>(p3.x);
			mesh[k + 13] = static_cast<float>(p3.y);
			mesh[k + 14] = static_cast<float>(p3.z);
			mesh[k + 15] = static_cast<float>(n.x);
			mesh[k + 16] = static_cast<float>(n.y);
			mesh[k + 17] = static_cast<float>(n.z);
			k += 18;
			triNum += 18;


		}
	}

	for (int j = res.y - 1; j > 0; j--) {
		for (int i = 1; i < res.x; ++i) {
			auto pos1 = posFunc(i, j);
			auto tempX1 = (pos1.x - 1) * temp_k;
			auto tempY1 = (h->lookAt(i, j) - 1) * temp_k;
			auto tempZ1 = (pos1.y - 1) * temp_k;
			Vector3D p1(tempX1, tempY1, tempZ1);
			//writer.write_in_ply(pos1.x, h->lookAt(i, j), pos1.y);


			auto pos2 = posFunc(i, j - 1);
			auto tempX2 = (pos2.x - 1) * temp_k;
			auto tempY2 = (h->lookAt(i, j - 1) - 1) * temp_k;
			auto tempZ2 = (pos2.y - 1) * temp_k;
			Vector3D p2(tempX2, tempY2, tempZ2);


			auto pos3 = posFunc(i - 1, j - 1);
			auto tempX3 = (pos3.x - 1) * temp_k;
			auto tempY3 = (h->lookAt(i - 1, j - 1) - 1) * temp_k;
			auto tempZ3 = (pos3.y - 1) * temp_k;
			Vector3D p3(tempX3, tempY3, tempZ3);


			auto n = (p2 - p1).cross(p3 - p2).getNormalize();

			mesh[k + 0] = static_cast<float>(p1.x);
			mesh[k + 1] = static_cast<float>(p1.y);
			mesh[k + 2] = static_cast<float>(p1.z);
			mesh[k + 3] = static_cast<float>(n.x);
			mesh[k + 4] = static_cast<float>(n.y);
			mesh[k + 5] = static_cast<float>(n.z);
			mesh[k + 6] = static_cast<float>(p2.x);
			mesh[k + 7] = static_cast<float>(p2.y);
			mesh[k + 8] = static_cast<float>(p2.z);
			mesh[k + 9] = static_cast<float>(n.x);
			mesh[k + 10] = static_cast<float>(n.y);
			mesh[k + 11] = static_cast<float>(n.z);
			mesh[k + 12] = static_cast<float>(p3.x);
			mesh[k + 13] = static_cast<float>(p3.y);
			mesh[k + 14] = static_cast<float>(p3.z);
			mesh[k + 15] = static_cast<float>(n.x);
			mesh[k + 16] = static_cast<float>(n.y);
			mesh[k + 17] = static_cast<float>(n.z);
			k += 18;
			triNum += 18;



		}
	}

	return triNum;
}