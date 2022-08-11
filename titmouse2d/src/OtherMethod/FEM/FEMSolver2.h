#include "FEMData2.h"

class FEMSolver2 {
public:
	FEMSolver2(std::initializer_list<Vector2D> vertexs,
		std::initializer_list<int> indexs) {
		femData = std::make_shared<FEMData2>(vertexs, indexs);
	}

	double calculateTriArea(int idx)const {
		auto r1 = femData->mesh->at(idx, 1) - femData->mesh->at(idx, 0);
		auto r2 = femData->mesh->at(idx, 2) - femData->mesh->at(idx, 0);
		return 0.5 * std::abs(r1.cross(r2));
	}

	//只执行一次
	void initOriginEdgeMat() {
		auto& mesh = femData->mesh;
		for (int i = 0; i < mesh->size(); ++i) {
			auto& originEdgeMat = femData->originEdgeMatrix[i];
			auto x10 = mesh->at(i, 0) - femData->mesh->at(i, 1);
			auto x20 = mesh->at(i, 0) - femData->mesh->at(i, 2);
			originEdgeMat(0, 0) = x10.x;
			originEdgeMat(0, 1) = x20.x;
			originEdgeMat(1, 0) = x10.y;
			originEdgeMat(1, 1) = x20.y;
			//这里可作优化，直接优化成求逆
			originEdgeMat = originEdgeMat.inverse();
		}
	}

	void calculateDeformGradMat() {
		auto& mesh = femData->mesh;
		for (int i = 0; i < mesh->size(); ++i) {
			auto& deformGradMat = femData->deformGradMatrix[i];
			auto x10 = mesh->at(i, 0) - femData->mesh->at(i, 1);
			auto x20 = mesh->at(i, 0) - femData->mesh->at(i, 2);
			deformGradMat(0, 0) = x10.x;
			deformGradMat(0, 1) = x20.x;
			deformGradMat(1, 0) = x10.y;
			deformGradMat(1, 1) = x20.y;

			deformGradMat = deformGradMat * femData->originEdgeMatrix[i];
		}
	}

	void calculateGreenStrainMat() {
		auto& mesh = femData->mesh;
		for (int i = 0; i < mesh->size(); ++i) {
			auto& S = femData->strainMatrix[i];
			auto& F = femData->deformGradMatrix[i];
			auto G = 0.5 * (F.transpose() * F - Matrix2x2<double>::identityMatrix());

			auto s11 =

		}
	}


public:
	FEMData2Ptr femData;
};

