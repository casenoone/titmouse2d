#include "FEMData2.h"

class FEMSolver2 {
public:
	FEMSolver2(std::initializer_list<Vector2D> vertexs,
		std::initializer_list<int> indexs) {
		femData = std::make_shared<FEMData2>(vertexs, indexs);
		initOriginEdgeMat();
		calculateTriArea();
	}

	//只执行一次
	void calculateTriArea()const {
		auto& mesh = femData->mesh;
		auto& areas = femData->areas;
		for (int i = 0; i < mesh->size(); ++i) {
			auto r1 = femData->mesh->at(i, 1) - femData->mesh->at(i, 0);
			auto r2 = femData->mesh->at(i, 2) - femData->mesh->at(i, 0);
			areas[i] = 0.5 * std::abs(r1.cross(r2));
		}
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
		auto mu = femData->mu;
		auto lambda = femData->lambda;
		for (int i = 0; i < mesh->size(); ++i) {
			auto& S = femData->strainMatrix[i];
			auto& F = femData->deformGradMatrix[i];
			auto G = 0.5 * (F.transpose() * F - Matrix2x2<double>::identityMatrix());

			S(0, 0) = 2 * mu * G(0, 0) + lambda * G(0, 0);;
			S(0, 1) = 2 * mu * G(0, 1);
			S(1, 0) = 2 * mu * G(1, 0);
			S(1, 1) = 2 * mu * G(1, 1) + lambda * G(1, 1);
		}
	}

	void calculateForces() {
		auto& mesh = femData->mesh;
		auto& F = femData->deformGradMatrix;
		auto& S = femData->strainMatrix;
		auto& O = femData->originEdgeMatrix;
		auto& forces = femData->forces;
		auto& areas = femData->areas;
		for (int i = 0; i < mesh->size(); ++i) {
			auto result = -areas[i] * F[i] * S[i] * O[i];
			auto f1 = Vector2D(result(0, 0), result(1, 0));
			auto f2 = Vector2D(result(0, 1), result(1, 1));
			auto f0 = -1.0 * (f1 + f2);

			auto i0 = mesh->triList[i].index[0];
			auto i1 = mesh->triList[i].index[1];
			auto i2 = mesh->triList[i].index[2];

			forces[i0] += f0;
			forces[i1] += f1;
			forces[i2] += f2;

		}
	}

	void timeIntegration(double dt) {
		auto& forces = femData->forces;
		auto& vels = femData->velocities;
		auto& pos = femData->mesh->vertexList;
		for (int i = 0; i < vels.size(); ++i) {
			vels[i] += (forces[i] + Vector2D(0, -9.8)) * dt;
			pos[i] += vels[i] * dt;
		}
	}

	void onAdvanceTimeStep(double dt) {
		clearForces();
		calculateDeformGradMat();
		calculateGreenStrainMat();
		calculateForces();
		//accumulateDamping();
		timeIntegration(dt);
		solveCollision();
	}

	void accumulateDamping() {
		auto& forces = femData->forces;
		auto& vels = femData->velocities;
		for (int i = 0; i < vels.size(); ++i) {
			vels[i] = vels[i] - 0.06 * vels[i];
		}

	}

	void clearForces() {
		auto& forces = femData->forces;
		//clear会使capacity变为0吗
		forces.clear();
		forces.resize(femData->mesh->vertexList.size());
	}

	void solveCollision() {
		auto& pos = femData->mesh->vertexList;
		auto& vels = femData->velocities;

		for (int i = 0; i < pos.size(); ++i) {
			if (pos[i].x > 2) {
				pos[i].x = 2;
			}
			if (pos[i].y > 2) {
				pos[i].y = 2;
			}
			if (pos[i].x < 0.01) {
				pos[i].x = 0.01;
			}
			if (pos[i].y < 0.01) {
				pos[i].y = 0.01;
			}

		}
	}


public:
	FEMData2Ptr femData;
};

