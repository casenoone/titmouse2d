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


const double vorticity_eps = 0.00001;

const Vector2D vs_vec = Vector2D(2.0, 0.0);

double vs_tau = 1.0;

double fv_eps = 0.00000001;

//newPosition赋值给positions可以改用移动构造函数，提高效率

FoamVortexSolver::FoamVortexSolver() {
	_particleSystemData = make_shared<FoamVortexData>();
	_foamVortexData = make_shared<FoamVortexData>();
	_foamVortexData = std::dynamic_pointer_cast<FoamVortexData>(_particleSystemData);
}

FoamVortexSolver::~FoamVortexSolver() {

}

void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {

	auto n = _foamVortexData->numberOfParticles();
	auto& positions = _foamVortexData->positions();
	//auto& forces = _foamVortexData->forces();
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
		//votexSheetVel = computeSingleVelocityFromPanels(i);

		_newVelocities[i] = vs_vec + votexSheetVel;
		_newPositions[i] = positions[i] + timeIntervalInSeconds * (_newVelocities[i] + vortexVel);
	}


}

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {
	vortexSheetSolve(timeIntervalInSeconds);
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

void FoamVortexSolver::setPanels(ExplicitSurface2Ptr surfaces) {
	_foamVortexData->panelSet = surfaces;
	computeBoundaryMatrix();
}

void FoamVortexSolver::setMovingGrid(const Vector2I& resolution_,
	const BoundingBox2& domain_) {
	_foamVortexData->movingGrid = make_shared<MovingFaceCenteredGrid2>(resolution_, domain_);
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
	if (step % 20 == 0 && n < 10000) {


		auto& pos = data->positions();
		auto& vel = data->velocities();
		auto panels = data->panelSet;

		auto emitParticle = data->newParticles;
		auto emitNum = emitParticle.dataSize();

		auto gamma = data->strength;

		Vector2D tempPos;

		for (int i = 0; i < emitNum; ++i) {
			pos.push(emitParticle(i));
			vel.push(Vector2D(0.0, 0.0));
			double temp_vorticity = 0;
			if (gamma.size() > 0)
				temp_vorticity = gamma[i];
			data->vorticities().push(temp_vorticity);
			_newVelocities.push(Vector2D(0.0, 0.0));
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}


	}

	step++;
}

//计算index点处的速度值
//这里的index是流体粒子的index
Vector2D FoamVortexSolver::computeSingleVelocityFromPanels(int index) {
	return Vector2D::zero();
}


//计算在pos处引发的诱导速度
Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(const Vector2D& pos, int index) {
	auto position = _foamVortexData->newParticles;
	auto r2 = (pos - position[index]).getLengthSquared();
	auto uv = Vector2D(position[index].y - pos.y, pos.x - position[index].x);
	return uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));
}



//只执行一次
void FoamVortexSolver::computeBoundaryMatrix() {
	auto panels = _foamVortexData->panelSet;
	auto& emittedParticles = _foamVortexData->newParticles;
	auto panelSize = panels->size();
	auto emitNum = panelSize * 5;
	emittedParticles.reSize(emitNum);

	Vector2D tempPos;
	for (int i = 0; i < panelSize; ++i) {
		for (int j = 0; j < 5; ++j) {
			auto line = panels->lookAt(i).end - panels->lookAt(i).start;
			auto lambda = random_double(0, 1);
			tempPos = panels->lookAt(i).start + lambda * line;
			tempPos += panels->lookAt(i).normal * random_double(0.02, 0.03);
			emittedParticles(i * 5 + j) = tempPos;
		}
	}

	Eigen::MatrixXd& A = _foamVortexData->A;

	A.resize(panelSize, emitNum);
	for (int i = 0; i < panelSize; ++i) {
		auto pos = panels->midPoint(i);
		auto normal = panels->lookAt(i).normal;
		auto tangent = Vector2D(-normal.y, normal.x);
		for (int j = 0; j < emitNum; ++j) {
			auto vel = computeUnitVelocityFromPanels(pos, j);
			A(i, j) = vel.dot(tangent);
		}
	}

}





//在这里求解vortex sheet strength
void FoamVortexSolver::vortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto panelSize = panels->size();

	auto& pos = data->positions();
	Eigen::MatrixXd& A = _foamVortexData->A;
	Eigen::VectorXd& x = _foamVortexData->strength;

	//组装b
	Eigen::VectorXd b(panelSize);

	for (int i = 0; i < panelSize; ++i) {
		auto pos = panels->midPoint(i);
		auto normal = panels->lookAt(i).normal;
		auto tangent = Vector2D(-normal.y, normal.x);

		Vector2D tempVel;
		//for (int j = 0; j < n; ++j) {
			//tempVel += computeUSingle(pos, j);
		//}
		b[i] = (Vector2D::zero() - vs_vec - tempVel).dot(tangent);
	}

	x = A.colPivHouseholderQr().solve(b);
}