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

const Vector2D vs_vec = Vector2D(0.6, 0.0);

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
		//for (int j = 0; j < n; ++j) {
		//	if (i != j) {
		//		//这个需要在data类里新增一个成员变量
		//		//所以当需要某个时刻粒子的速度时
		//		//返回的应该是velocity + vortexVel
		//		vortexVel += computeUSingle(positions[i], j);
		//	}
		//}

		Vector2D votexSheetVel;

		//在这里采样边界速度
		votexSheetVel = computeSingleVelocityFromPanels(i);
		//cout << votexSheetVel.x << endl;
		//cout << "边界速度：" << votexSheetVel.x << endl;
		//暂时不考虑受力
		//需要注意的是，这个速度积分并不包括上面从涡量场恢复出来的速度场
		//_newVelocities[i] = vel[i] + votexSheetVel /* + forces[i] * timeIntervalInSeconds */;
		_newVelocities[i] = vs_vec + votexSheetVel;
		_newPositions[i] = positions[i] + timeIntervalInSeconds * (_newVelocities[i] + vortexVel);
	}


}

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//onBeginAdvanceTimeStep();
	//transferFromParticlesToGrids();
	vortexSheetSolve(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	//ParticleSystemSolver2::resolveCollision();
	onEndAdvanceTimeStep();
	//emitParticles();
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
	correctPanelCoordinateSystem();
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

	double dx = 0.05;
	double dy = 0.05;
	Vector2D center(0.1, 1.0);
	Vector2D lower(center.x - dx, center.y - dy);
	Vector2D upper(center.x + dx, center.y + dy);

	int emitNum = 10;
	Vector2D tempPos;
	for (int i = 0; i < emitNum; ++i) {

		tempPos.x = random_double(lower.x, upper.x);
		tempPos.y = random_double(lower.y, upper.y);
		//tempPos.x = 0.05;
		//tempPos.y = 0.9;
		pos.push(tempPos);
		vel.push(Vector2D(0.0, 0.0));
		data->vorticities().push(0.0);
		_newVelocities.push(Vector2D(0.0, 0.0));
		_newPositions.push(Vector2D());

		data->numberOfParticles()++;
	}


}

void FoamVortexSolver::emitParticlesFromPanel() {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto& pos = data->positions();
	auto& vel = data->velocities();

	double dx = 0.3;
	double dy = 0.4;
	Vector2D center(0.5, 1.0);
	Vector2D lower(center.x - dx, center.y - dy);
	Vector2D upper(center.x + dx, center.y + dy);

	int emitNum = 1;
	Vector2D tempPos;
	for (int i = 0; i < emitNum; ++i) {

		tempPos.x = random_double(lower.x, upper.x);
		tempPos.y = random_double(lower.y, upper.y);
		//tempPos.x = 0.05;
		//tempPos.y = 0.9;
		pos.push(tempPos);
		vel.push(Vector2D(0.0, 0.0));
		data->vorticities().push(0.0);
		_newVelocities.push(Vector2D(0.0, 0.0));
		_newPositions.push(Vector2D());

		data->numberOfParticles()++;
	}


}


//计算index点处的速度值
//这里的index是流体粒子的index
Vector2D FoamVortexSolver::computeSingleVelocityFromPanels(int index) {
	auto& pos = _foamVortexData->positions();
	auto panel = _foamVortexData->panelSet;
	auto panelSize = panel->size();
	auto& gama = _foamVortexData->strength;

	Vector2D result;
	for (int i = 0; i < panelSize; ++i) {

		auto start = panel->lookAt(i).start;
		auto end = panel->lookAt(i).end;
		auto normal = panel->lookAt(i).normal;

		Vector2D temp2;

		//首先，组装坐标变换矩阵
		//这里可进行优化
		auto transToLocal = Matrix3x3<double>::transToLocalMatrix(normal, start);
		auto transToWorld = transToLocal.inverse();

		double beta = 0.0;
		auto vec_r1 = start - pos[index];
		auto vec_r2 = end - pos[index];
		auto r1 = vec_r1.getLength();
		auto r2 = vec_r2.getLength();
		auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
		//beta = acos(std::min(std::max(temp1, -1.0), 1.0));
		beta = acos(temp1);
		if (isnan(beta)) {
			beta = kPiD;
		}

		//cout << gama[i] << endl;
		temp2.x = beta / (2.0 * kPiD);
		temp2.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);

		//temp2 = transToWorld * temp2 * gama[i];
		temp2 = temp2 * gama[i];
		temp2 = vel_to_world(temp2, start, normal);
		result += temp2;
	}
	return result;
}



//这里的index是panel的index
Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
	auto panel = _foamVortexData->panelSet;
	auto start = panel->lookAt(index).start;
	auto end = panel->lookAt(index).end;
	auto normal = panel->lookAt(index).normal;

	//首先，组装坐标变换矩阵
	//auto transToLocal = Matrix3x3<double>::transToLocalMatrix(normal, start);
	//auto transToWorld = transToLocal.inverse();

	Vector2D result;

	//计算beta值.
	double beta = 0.0;
	auto vec_r1 = start - midPoint;
	auto vec_r2 = end - midPoint;
	auto r1 = vec_r1.getLength();
	auto r2 = vec_r2.getLength();
	auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
	//beta = acos(std::min(std::max(temp1, -1.0), 1.0));
	beta = acos(temp1);
	if (isnan(beta)) {
		beta = kPiD;
	}
	//eq(16) eq(17)
	//按照论文上的逻辑来看，这里算出来的是局部坐标系下的表示
	result.x = beta / (2.0 * kPiD);
	result.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);

	//result = transToWorld * result;
	result = vel_to_world(result, start, normal);
	//cout << result.x << endl;
	return result;
}




//要确保normal x (end - start) < 0
void FoamVortexSolver::correctPanelCoordinateSystem() {
	auto panels = _foamVortexData->panelSet;
	auto size = panels->size();

	for (int i = 0; i < size; ++i) {
		auto& data = (*panels)[i];
		if (data.normal.cross(data.end - data.start) > 0) {

			//std::swap(data.end, data.start);
		}
		else {
			//cout << i << endl;
		}
	}
}

//只执行一次
void FoamVortexSolver::computeBoundaryMatrix() {
	auto panels = _foamVortexData->panelSet;
	auto panelSize = panels->size();
	Eigen::MatrixXd& A = _foamVortexData->A;

	auto sizex = panelSize + 1;
	auto sizey = panelSize;
	A.resize(sizex, sizey);

	for (int j = 0; j < panelSize; ++j) {
		auto normal = panels->lookAt(j).normal;
		for (int i = 0; i < panelSize; ++i) {
			auto mid_j = panels->midPoint(j);
			auto u_ji = computeUnitVelocityFromPanels(i, mid_j).dot(normal);
			//cout << "A:" << computeUnitVelocityFromPanels(j, mid_i).x << "," << computeUnitVelocityFromPanels(j, mid_i).y << endl;
			//cout << "B:" << normal.x << "," << normal.y << endl;
			//cout << "C:" << u_ji << endl;
			//cout << computeUnitVelocityFromPanels(j, mid_i).x << endl;
			//cout << mid_i.x << endl;
			//cout << normal.y << endl;
			//cout << u_ji << endl;
			A(j, i) = -u_ji;
		}
	}

	for (int i = 0; i < panelSize; ++i) {
		//cout << panels->lookAt(i).start.x << "," << panels->lookAt(i).start.y << ","
		//	<< panels->lookAt(i).end.x << "," << panels->lookAt(i).end.y << endl;
		//cout << panels->midPoint(i).x << "," << panels->midPoint(i).y << endl;
		//cout << panels->lookAt(i).normal.x << endl;
		A(panelSize, i) = 1.0;
	}


	//cout << A << endl;
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
	auto boundaryVel = data->movingGrid;

	//组装b
	Eigen::VectorXd b(panelSize + 1);
	for (int i = 0; i < panelSize; ++i) {
		auto normal = panels->lookAt(i).normal;
		auto vec = vs_vec;
		b[i] = vec.dot(normal);
	}

	b[panelSize] = 0;

	auto k = 0;
	//x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
	x = A.colPivHouseholderQr().solve(b);

	for (int i = 0; i < panelSize; ++i) {
		k += x[i];
	}
	//if (k != 0)
	//	cout << k << endl;
	////cout << x << endl;



	/*cout << "start" << endl;
	cout << x << endl;
	cout << "b" << endl;
	cout << b << endl;
	cout << "A" << endl;
	cout << A << endl;
	cout << "end" << endl;*/

}