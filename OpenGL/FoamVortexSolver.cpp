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

static const int numOfStep = 4;

static const double vorticity_eps = 0.02;

static const Vector2D vs_vec = Vector2D(2.5, 0.0);

static double vs_tau = 1.0;

static double fv_eps = 0.00000001;

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


void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {

	auto& vortex_pos = _foamVortexData->vortexPosition;
	auto& vortex_vel = _foamVortexData->vortexVelocity;
	auto n = vortex_pos.dataSize();
	Array<Vector2D> tempP(n);

	for (int i = 0; i < n; ++i) {
		Vector2D vortexVel;
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				vortexVel += computeUSingle(vortex_pos[i], j);
			}
		}
		tempP[i] = vortex_pos[i] + timeIntervalInSeconds * vortexVel;
	}
	//这里搞一个移动赋值函数
	vortex_pos = tempP;


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
	emitParticlesFromPanels(timeIntervalInSeconds);
	_shallowWaveSolver->onAdvanceTimeStep(timeIntervalInSeconds);
}


FoamVortexDataPtr& FoamVortexSolver::foamVortexData() {
	return _foamVortexData;
}


Vector2D FoamVortexSolver::computeUSingle(const Vector2D& pos, int i)const {

	auto position = _foamVortexData->vortexPosition;
	auto gamma = _foamVortexData->gamma();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return gamma[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));

}



void FoamVortexSolver::setMovingBoudnary(RegularPolygonPtr surfaces) {
	_foamVortexData->panelSet = surfaces;
	constructMovingBoundaryMatrix();
}


void FoamVortexSolver::setShallowWaveMovingBoundary(const Vector2D& center, const double r) {
	_shallowWaveSolver->setSphereMarkers(center, r);
}


void FoamVortexSolver::emitParticlesFromPanels(double timeIntervalInSeconds) {

	static int step = 0;
	auto data = _foamVortexData;
	auto& pos = _foamVortexData->vortexPosition;
	auto panelVel = data->panelSet->velocity;
	auto panel = data->panelSet;
	auto& emitParticle = data->newParticles;

	auto emitNum = emitParticle.dataSize();

	//更改粒子发射位置使得与panel同步
	for (int i = 0; i < emitNum; ++i) {
		emitParticle[i] += panelVel * timeIntervalInSeconds;
	}

	if (step % 4 == 0 && pos.dataSize() < 100000 && (panel->center().x - panel->r()) < 1.2) {
		auto& pos = data->vortexPosition;
		auto& vel = data->vortexVelocity;
		auto panels = data->panelSet;
		auto gamma = data->slip_strength;

		Vector2D tempPos;

		auto startNum = pos.dataSize();

		//push_back这里可作优化
		for (int i = 0; i < emitNum; ++i) {
			pos.push(emitParticle(i));
			vel.push(Vector2D(0.0, 0.0));
			double temp_gamma = 0;
			data->gamma().push(temp_gamma);
		}


		Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
		Eigen::VectorXd& x = _foamVortexData->slip_strength;

		auto panelSize = data->panelSet->size();
		auto panelVelocity = data->panelSet->velocity;
		//组装b
		Eigen::VectorXd b(panelSize * 2);

		for (int i = 0; i < panelSize; ++i) {
			auto temp_pos = panels->midPoint(i);
			Vector2D tempVel;
			for (int j = 0; j < pos.dataSize(); ++j) {
				tempVel += computeUSingle(temp_pos, j);
			}
			auto v1 = (panelVelocity - tempVel);
			b[i] = v1.x;
			b[i + panelSize] = v1.y;
		}

		x = B * b;
		auto& vor = data->gamma();
		int j = 0;
		for (int i = startNum; i < pos.dataSize(); ++i) {
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
		tracerPos[i] = tempPos;
	}
}


//求解tracer粒子的运动
void FoamVortexSolver::tracerParticlesSolve() {
	auto& tracerPos = _foamVortexData->tracePosition;
	auto& tracerVel = _foamVortexData->traceVelocity;
	auto n = tracerPos.dataSize();

	auto vor_n = _foamVortexData->vortexPosition.dataSize();
	for (int i = 0; i < n; ++i) {
		Vector2D tempVel;
		Vector2D pos = tracerPos(i);
		for (int j = 0; j < vor_n; ++j) {
			tempVel += computeUSingle(pos, j);
		}
		tracerVel(i) = tempVel;
	}
}


void FoamVortexSolver::constructStaticBoundaryMatrix() {

}



//只执行一次
void FoamVortexSolver::constructMovingBoundaryMatrix() {
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

	Eigen::MatrixXd A;

	A.resize(panelSize * 2, emitNum);
	for (int i = 0; i < panelSize; ++i) {
		auto pos = panels->midPoint(i);
		for (int j = 0; j < emitNum; ++j) {
			auto vel = computeUnitVelocityFromPanels(pos, j);
			A(i, j) = vel.x;
			A(i + panelSize, j) = vel.y;
		}
	}



	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
	Eigen::MatrixXd A_trans = A.transpose();

	auto I = Eigen::MatrixXd::Identity(emitNum, emitNum);

	B = (A_trans * A + 3 * I).inverse() * A_trans;


	//在这里构建静态边界条件


}


