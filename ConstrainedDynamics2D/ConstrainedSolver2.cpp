#include "ConstrainedSolver2.h"

void ConstrainedSolver2::timeIntegration(double dt) {

}

//CompliantMat的尺寸：ExE
//E:约束的数量
//暂时使用稠密矩阵吧
void ConstrainedSolver2::constructCompliantMat() {
	auto& edge = massSpringData->edges;
	auto edgeNum = edge.dataSize();
	auto c = 1 / massSpringData->stiff;
	auto& compliantMat = massSpringData->CompliantMat;
	compliantMat.resize(edgeNum, edgeNum);
	int j = 0;
	for (int i = 0; i < edgeNum; ++i) {
		compliantMat(i, j++) = c;
	}
}


void ConstrainedSolver2::constructJacobinMat() {

}

void ConstrainedSolver2::constructConstraint() {
	auto& pos = massSpringData->positions;
	auto n = massSpringData->numberOfPoint;
	auto& edge = massSpringData->edges;
	auto restLen = massSpringData->restLen;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			auto& pos_i = pos[i];
			auto& pos_j = pos[j];
			//如果满足这个条件就建立约束
			if (pos_i.dis(pos_j) <= restLen + 0.1) {
				edge.push(MassSpringData2::Edge{ i,j });
			}
		}
	}
}

void ConstrainedSolver2::onAdvanceTimeStep(double dt) {

}