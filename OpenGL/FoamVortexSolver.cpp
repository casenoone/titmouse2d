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

static const int numOfStep = 0;
static const double vorticity_eps = 0.06;
static double fv_eps = 0.001;
static int static_boudary_interval = 20;

Vector2D vel_to_world(const Vector2D vel_local, const Vector2D n_i, const Vector2D normal_i) {
	double cos_theta = normal_i.y; // cos(theta)
	double sin_theta = normal_i.x; // sin(theta)
	Vector2D vel = Vector2D(cos_theta * vel_local.x + sin_theta * vel_local.y,
		-sin_theta * vel_local.x + cos_theta * vel_local.y);
	return vel;
}

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
		Vector2D votexSheetVel;
		votexSheetVel = static_computeSingleVelocityFromPanels(i);
		//std::cout << votexSheetVel.x << std::endl;

		//votexSheetVel = Vector2D::zero();
		tempP[i] = vortex_pos[i] + timeIntervalInSeconds * (votexSheetVel + vortexVel);
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

	no_through_solve(timeIntervalInSeconds);
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

void FoamVortexSolver::setStaticBoudnary(ExplicitSurface2Ptr surfaces) {
	auto box = surfaces;
	auto& staticBoundary = _foamVortexData->staticBoundary;
	int num = 0;
	for (int i = 0; i < 4; ++i) {
		auto start = box->lookAt(i).end;
		auto end = box->lookAt(i).start;
		auto normal = box->lookAt(i).normal;

		std::cout << normal.x << "," << normal.y << std::endl;
		//std::cout << start.x << "," << start.y << std::endl;


		auto line = end - start;
		auto length = line.getLength();
		line = line.getNormalize();

		double interval = length / (static_boudary_interval);
		for (int j = 0; j < static_boudary_interval; ++j) {
			auto temp_start = start + j * interval * line;
			auto temp_end = start + (j + 1.0) * interval * line;
			FoamVortexData::Panel panel(temp_start, temp_end, 0.5 * (temp_start + temp_end), normal);
			staticBoundary.push_back(panel);
			//std::cout << temp_start.x << "," << temp_start.y << std::endl;
			//std::cout << temp_end.x << "," << temp_end.y << std::endl;

		}

	}

	constructStaticBoundaryMatrix();
}


void FoamVortexSolver::setShallowWaveMovingBoundary(const Vector2D& center, const double r) {
	_shallowWaveSolver->setSphereMarkers(center, r);
}

void FoamVortexSolver::emitVortexRing() {
	auto& pos = _foamVortexData->vortexPosition;
	auto& vel = _foamVortexData->vortexVelocity;
	auto& gamma = _foamVortexData->gamma();
	int n = 4;
	pos.reSize(n);
	vel.reSize(n);
	gamma.reSize(n);
	Vector2D A(0.2, 1.2);
	Vector2D B(0.2, 1.1);
	Vector2D C(0.2, 1.0);
	Vector2D D(0.2, 0.9);

	pos[0] = A;
	pos[1] = B;
	pos[2] = C;
	pos[3] = D;

	gamma[0] = 0.6;
	gamma[1] = 0.6;
	gamma[2] = -0.6;
	gamma[3] = -0.6;
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

	//int emitNum = 10000;
	int emitNum = 1;
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


//这里的index是panel的index
Vector2D FoamVortexSolver::static_computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
	auto& panel = _foamVortexData->staticBoundary;
	auto start = panel[index].start;
	auto end = panel[index].end;
	auto normal = panel[index].normal;

	Vector2D result;

	//计算beta值.
	double beta = 0.0;
	auto vec_r1 = start - midPoint;
	auto vec_r2 = end - midPoint;
	auto r1 = vec_r1.getLength();
	auto r2 = vec_r2.getLength();
	auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
	beta = acos(temp1);
	if (isnan(beta)) {
		beta = kPiD;
	}
	//eq(16) eq(17)
	result.x = beta / (2.0 * kPiD);
	result.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);

	result = vel_to_world(result, start, normal);
	return result;
}


Vector2D FoamVortexSolver::static_computeSingleVelocityFromPanels(int index) {
	auto& pos = _foamVortexData->vortexPosition;
	auto& panel = _foamVortexData->staticBoundary;
	auto panelSize = panel.size();
	auto& gama1 = _foamVortexData->no_through_strength;

	Vector2D result;

	for (int i = 0; i < panelSize; ++i) {

		auto start = panel[i].start;
		auto end = panel[i].end;
		auto normal = panel[i].normal;

		Vector2D temp2;

		double beta = 0.0;
		auto vec_r1 = start - pos[index];
		auto vec_r2 = end - pos[index];
		auto r1 = vec_r1.getLength();
		auto r2 = vec_r2.getLength();
		auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
		beta = acos(temp1);
		if (isnan(beta)) {
			beta = kPiD;
		}

		temp2.x = beta / (2.0 * kPiD);
		temp2.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);

		double tempgamma = 0.0;
		if (gama1.size() > 0)
			tempgamma = gama1[i];
		temp2 = temp2 * tempgamma;
		temp2 = vel_to_world(temp2, start, normal);
		result += temp2;
	}

	return result;
}



void FoamVortexSolver::no_through_solve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto& pos = data->vortexPosition;

	auto n = pos.dataSize();
	auto& panels = data->staticBoundary;
	auto panelSize = panels.size();
	Eigen::MatrixXd& A = _foamVortexData->no_through_matrix;
	Eigen::VectorXd& x = _foamVortexData->no_through_strength;
	//组装b
	Eigen::VectorXd b(panelSize + 1);
	for (int i = 0; i < panelSize; ++i) {
		auto normal = panels[i].normal;
		auto pos = panels[i].mid;

		Vector2D temp;
		for (int j = 0; j < n; ++j) {
			temp += computeUSingle(pos, j);
		}
		b[i] = temp.dot(normal);
	}

	b[panelSize] = 0;
	x = A.colPivHouseholderQr().solve(b);
}



void FoamVortexSolver::constructStaticBoundaryMatrix() {

	auto& panel = _foamVortexData->staticBoundary;
	auto num = panel.size();
	Eigen::MatrixXd& mat = _foamVortexData->no_through_matrix;
	auto sizex = num + 1;
	auto sizey = num;
	mat.resize(sizex, sizey);
	for (int j = 0; j < num; ++j) {
		auto normal = panel[j].normal;
		auto pos = panel[j].mid;
		for (int i = 0; i < num; ++i) {
			auto u_ji = static_computeUnitVelocityFromPanels(i, pos).dot(normal);
			mat(j, i) = -u_ji;
		}
	}

	for (int i = 0; i < num; ++i) {
		mat(num, i) = 1.0;
	}
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




}


