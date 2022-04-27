#include "FoamVortexSolver.h"

#include "../titmouse2d/src/ConstVar.h"

#include "../titmouse2d/src/random.h"

#include "../titmouse2d/src/Matrix3x3.hpp"

#include "../titmouse2d/src/boundingbox2.h"

#include "../titmouse2d/src/LinearSystem/JacobiSolver.hpp"

#include "../titmouse2d/src/LinearSystem/ConjugateGradientSolver.hpp"
#include <omp.h>

#include <iostream>
#include <cmath>
#include <windows.h>
#include <Eigen/SVD>
#include <Eigen/LU>



Vector2D vel_to_world(const Vector2D vel_local, const Vector2D n_i, const Vector2D normal_i) {
	double cos_theta = normal_i.y; // cos(theta)
	double sin_theta = normal_i.x; // sin(theta)
	Vector2D vel = Vector2D(cos_theta * vel_local.x + sin_theta * vel_local.y,
		-sin_theta * vel_local.x + cos_theta * vel_local.y);
	return vel;
}

static const int numOfStep = 5;

static const double vorticity_eps = 0.04;

static const Vector2D vs_vec = Vector2D(2.5, 0.0);

static double vs_tau = 1.0;

static double fv_eps = 0.00000001;

//newPosition赋值给positions可以改用移动构造函数，提高效率

FoamVortexSolver::FoamVortexSolver() {
	_particleSystemData = std::make_shared<FoamVortexData>();
	_foamVortexData = std::make_shared<FoamVortexData>();
	_foamVortexData = std::dynamic_pointer_cast<FoamVortexData>(_particleSystemData);
}

FoamVortexSolver::FoamVortexSolver(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) :FoamVortexSolver() {

	_shallowWaveSolver = std::make_shared<ShallowWaveSolver2>(resolution, gridSpacing, gridOrigin);
}


FoamVortexSolver::~FoamVortexSolver() {

}

void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {

	auto n = _foamVortexData->numberOfParticles();
	auto& positions = _foamVortexData->positions();
	auto& vel = _foamVortexData->velocities();

	for (int i = 0; i < n; ++i) {
		Vector2D vortexVel;
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				//这个需要在data类里新增一个成员变量
				//所以当需要某个时刻粒子的速度时
				//返回的应该是velocity + vortexVel
				vortexVel += computeUSingle(positions[i], j);
			}
		}

		Vector2D votexSheetVel;

		_newVelocities[i] = votexSheetVel;
		_newPositions[i] = positions[i] + timeIntervalInSeconds * (_newVelocities[i] + vortexVel);
	}


	//求解tracer粒子的速度场
	tracerParticlesSolve();


	//在这里更新tracer粒子
	auto tracerPos = _foamVortexData->tracePosition;
	auto traceVel = _foamVortexData->traceVelocity;
	auto tracer_n = tracerPos.dataSize();
	for (int i = 0; i < tracer_n; ++i) {
		tracerPos[i] += (traceVel[i]) * timeIntervalInSeconds;
	}

}

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {

	timeIntegration(timeIntervalInSeconds);
	onEndAdvanceTimeStep();
	emitParticlesFromPanels(timeIntervalInSeconds);
}


FoamVortexDataPtr& FoamVortexSolver::foamVortexData() {
	return _foamVortexData;
}


void FoamVortexSolver::onBeginAdvanceTimeStep() {
	ParticleSystemSolver2::beginAdvanceTimeStep();
}

void FoamVortexSolver::onEndAdvanceTimeStep() {
	ParticleSystemSolver2::endAdvanceTimeStep();
}


Vector2D FoamVortexSolver::computeUSingle(const Vector2D& pos, int i)const {

	auto position = _foamVortexData->positions();
	auto vorticity = _foamVortexData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));

}

void FoamVortexSolver::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_foamVortexData->vorticities().reSize(numberOfParticles);

	auto vorticity = _foamVortexData->vorticities();

	//为了方便测试，给每个粒子赋一个随机的涡量
	for (int i = 0; i < numberOfParticles; ++i) {
		//vorticity[i] = random_double(-0.01, 0.01);
		vorticity[i] = 0.000;
	}


}

void FoamVortexSolver::setPanels(RegularPolygonPtr surfaces) {
	_foamVortexData->panelSet = surfaces;
	computeBoundaryMatrix();
}

void FoamVortexSolver::setMovingGrid(const Vector2I& resolution_,
	const BoundingBox2& domain_) {
}




void FoamVortexSolver::emitParticles() {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto& pos = data->positions();
	auto& vel = data->velocities();
	auto panels = data->panelSet;

	auto gamma = data->strength;
	int emitNum = panels->size();
	Vector2D tempPos;

	for (int i = 0; i < emitNum; ++i) {
		for (int j = 0; j < 3; ++j) {

			auto line = panels->lookAt(i).end - panels->lookAt(i).start;
			double panelLength = line.getLength();
			auto lambda = random_double(0, panelLength);
			tempPos = panels->lookAt(i).start + lambda * line;
			tempPos += panels->lookAt(i).normal * random_double(0.01, 0.02);

			pos.push(tempPos);
			vel.push(Vector2D(0.0, 0.0));
			double temp_vorticity = 0;

			data->vorticities().push(temp_vorticity);
			_newVelocities.push(Vector2D(0.0, 0.0));
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}
	}


}


void FoamVortexSolver::emitParticlesFromPanels(double timeIntervalInSeconds) {

	static int step = 0;
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto panelVel = data->panelSet->velocity;
	auto panel = data->panelSet;
	auto& emitParticle = data->newParticles;

	auto emitNum = emitParticle.dataSize();

	//更改粒子发射位置使得与panel同步

	for (int i = 0; i < emitNum; ++i) {
		emitParticle[i] += panelVel * timeIntervalInSeconds;
	}

	if (step % 4 == 0 && n < 100000 && (panel->center().x - panel->r()) < 1.2) {


		auto& pos = data->positions();
		auto& vel = data->velocities();
		auto panels = data->panelSet;

		auto gamma = data->strength;

		Vector2D tempPos;

		auto startNum = data->numberOfParticles();

		for (int i = 0; i < emitNum; ++i) {
			pos.push(emitParticle(i));
			vel.push(Vector2D(0.0, 0.0));
			double temp_vorticity = 0;
			data->vorticities().push(temp_vorticity);
			_newVelocities.push(Vector2D(0.0, 0.0));
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}




		Eigen::MatrixXd& B = _foamVortexData->B;
		Eigen::VectorXd& x = _foamVortexData->strength;

		auto panelSize = data->panelSet->size();
		auto panelVelocity = data->panelSet->velocity;
		//组装b
		Eigen::VectorXd b(panelSize * 2);

		for (int i = 0; i < panelSize; ++i) {
			auto pos = panels->midPoint(i);
			Vector2D tempVel;
			for (int j = 0; j < n; ++j) {
				tempVel += computeUSingle(pos, j);
			}
			auto v1 = (panelVelocity - tempVel);
			b[i] = v1.x;
			b[i + panelSize] = v1.y;
		}

		x = B * b;
		auto& vor = data->vorticities();
		int j = 0;
		for (int i = startNum; i < data->numberOfParticles(); ++i) {
			vor[i] = x[j++];
		}
	}

	step++;
}



//计算在pos处引发的诱导速度
Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(const Vector2D& pos, int index) {

	auto position = _foamVortexData->newParticles;
	auto r2 = (pos - position[index]).getLengthSquared();
	auto uv = Vector2D(position[index].y - pos.y, pos.x - position[index].x);
	return uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));
}


//从边界上生成tracer粒子
//void FoamVortexSolver::emitTracerParticles() {
//	static int step = 0;
//	auto data = _foamVortexData;
//	auto& vorPos = _foamVortexData->positions();
//	auto& tracerPos = _foamVortexData->tracePosition;
//	auto n = tracerPos.dataSize();
//	auto panels = data->panelSet;
//
//	auto& tracerVel = _foamVortexData->traceVelocity;
//	int emitNum = 7;
//	if (step % 5 == 0 && n < 100000) {
//		Vector2D tempPos;
//		for (int i = 0; i < emitNum; ++i) {
//			for (int j = 0; j < panels->size(); ++j) {
//				if (panels->lookAt(j).normal.dot(Vector2D(1, 0)) > 0) {
//					auto line = panels->lookAt(j).end - panels->lookAt(j).start;
//					double panelLength = line.getLength();
//					auto lambda = random_double(0, 1);
//					tempPos = panels->lookAt(j).start + lambda * line;
//					tempPos += panels->lookAt(j).normal * random_double(0.01, 0.02);
//
//					tracerPos.push(tempPos);
//					tracerVel.push(Vector2D(0.0, 0.0));
//				}
//			}
//
//		}
//	}
//	step++;
//}



//移动边界所使用的tracer粒子
void FoamVortexSolver::emitTracerParticles() {
	static int step = 0;
	auto data = _foamVortexData;
	auto& tracerPos = _foamVortexData->tracePosition;
	auto& tracerVel = _foamVortexData->traceVelocity;
	auto n = tracerPos.dataSize();
	auto panels = data->panelSet;

	int emitNum = 10000;
	tracerPos.reSize(emitNum);
	tracerVel.reSize(emitNum);
	Vector2D tempPos;
	for (int i = 0; i < emitNum; ++i) {
		tempPos.x = random_double(0.2, 1.0);
		tempPos.y = random_double(0.5, 1.5);
		/*while ((tempPos - panels->center()).getLength() < panels->r()) {
			tempPos.x = random_double(0, 1);
			tempPos.y = random_double(0.7, 1.3);
		}*/
		tracerPos[i] = tempPos;
	}
}


//求解tracer粒子的运动
void FoamVortexSolver::tracerParticlesSolve() {
	auto& tracerPos = _foamVortexData->tracePosition;
	auto& tracerVel = _foamVortexData->traceVelocity;
	auto n = tracerPos.dataSize();

	auto vor_n = _foamVortexData->numberOfParticles();
	for (int i = 0; i < n; ++i) {
		Vector2D tempVel;
		Vector2D pos = tracerPos(i);
		for (int j = 0; j < vor_n; ++j) {
			tempVel += computeUSingle(pos, j);
		}
		tracerVel(i) = tempVel;
	}
}





//只执行一次
void FoamVortexSolver::computeBoundaryMatrix() {
	auto panels = _foamVortexData->panelSet;
	auto& emittedParticles = _foamVortexData->newParticles;
	auto panelSize = panels->size();
	auto emitNum = panelSize * numOfStep;
	emittedParticles.reSize(emitNum);

	Vector2D tempPos;
	for (int i = 0; i < panelSize; ++i) {
		for (int j = 0; j < numOfStep; ++j) {
			auto line = panels->lookAt(i).end - panels->lookAt(i).start;
			auto lambda = random_double(0, 1);
			tempPos = panels->lookAt(i).start + lambda * line;
			tempPos += panels->lookAt(i).normal * random_double(0.01, 0.02);
			emittedParticles(i * numOfStep + j) = tempPos;
		}
	}

	Eigen::MatrixXd& A = _foamVortexData->A;

	A.resize(panelSize * 2, emitNum);
	for (int i = 0; i < panelSize; ++i) {
		auto pos = panels->midPoint(i);
		for (int j = 0; j < emitNum; ++j) {
			auto vel = computeUnitVelocityFromPanels(pos, j);
			A(i, j) = vel.x;
			A(i + panelSize, j) = vel.y;
		}
	}



	Eigen::MatrixXd& B = _foamVortexData->B;
	Eigen::MatrixXd A_trans = A.transpose();

	auto I = Eigen::MatrixXd::Identity(emitNum, emitNum);

	B = (A_trans * A + 3 * I).inverse() * A_trans;


}


