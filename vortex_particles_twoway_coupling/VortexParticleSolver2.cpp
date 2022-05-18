#include "VortexParticleSolver2.h"

#include "../titmouse2d/src/ConstVar.h"

#include "../titmouse2d/src/random.h"

#include "../titmouse2d/src/Matrix3x3.hpp"

#include "../titmouse2d/src/boundingbox2.h"

#include <omp.h>

#include <iostream>
#include <cmath>
#include <windows.h>
#include <Eigen/SVD>
#include <Eigen/LU>



Vector2D vel_to_world(const Vector2D vel_local, const Vector2D n_i, const Vector2D normal_i) {
	double cos_theta = normal_i.y;
	double sin_theta = normal_i.x;
	Vector2D vel = Vector2D(cos_theta * vel_local.x + sin_theta * vel_local.y,
		-sin_theta * vel_local.x + cos_theta * vel_local.y);
	return vel;
}


const double vorticity_eps = 0.08;
//const double vorticity_eps = 0.0000001;

const Vector2D vs_vec = Vector2D(5, 0.0);

double vs_tau = 40.0;

double fv_eps = 0.00001;

//newPosition赋值给positions可以改用移动构造函数，提高效率

FoamVortexSolver::FoamVortexSolver() {
	_particleSystemData = std::make_shared<FoamVortexData>();
	_foamVortexData = std::make_shared<FoamVortexData>();
	_foamVortexData = std::dynamic_pointer_cast<FoamVortexData>(_particleSystemData);
}

FoamVortexSolver::~FoamVortexSolver() {

}

void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {

	auto& vortex_pos = _foamVortexData->vortexPosition;
	auto& vortex_vel = _foamVortexData->vortexVelocity;
	auto vortex_n = vortex_pos.dataSize();
	Array<Vector2D> tempP(vortex_n);

	for (int i = 0; i < vortex_n; ++i) {
		Vector2D vortexVel;
		for (int j = 0; j < vortex_n; ++j) {
			if (i != j) {
				//这个需要在data类里新增一个成员变量
				//所以当需要某个时刻粒子的速度时
				//返回的应该是velocity + vortexVel
				vortexVel += computeUSingle(vortex_pos[i], j);
			}
		}
		Vector2D votexSheetVel;
		//votexSheetVel = computeSingleVelocityFromPanels(i);
		tempP[i] = vortex_pos[i] + timeIntervalInSeconds * (votexSheetVel + vortexVel /* + vs_vec*/);

	}
	vortex_pos = tempP;

	tracerParticlesSolve();

	//在这里更新tracer粒子
	auto tracerPos = _foamVortexData->tracePosition;
	auto traceVel = _foamVortexData->traceVelocity;
	auto tracer_n = tracerPos.dataSize();

	for (int i = 0; i < tracer_n; ++i) {
		tracerPos[i] += traceVel[i] * timeIntervalInSeconds;
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

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {

	beginAdvanceTimeStep();

	//消去法向分量
	//vortexSheetSolve(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	//emitParticlesFromPanels(timeIntervalInSeconds);
	//消去切向分量
	//slipVortexSheetSolve(timeIntervalInSeconds);
	movingSlipVortexSheetSolve(timeIntervalInSeconds);
	all_movingSlipVortexSheetSolve(timeIntervalInSeconds);
	endAdvanceTimeStep();

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

Vector2D FoamVortexSolver::computeTwoWayForce(double timeIntervalInSeconds) {
	auto& panelset = _foamVortexData->panelSet;
	int len = panelset->size();

	double rho = 1.0;
	double c = 0.07;
	double dt = timeIntervalInSeconds;
	auto& gamma = _foamVortexData->slip_Strength;
	Vector2D f;
	for (int i = 0; i < len; ++i) {
		auto midPoint = panelset->midPoint(i);
		auto term1 = c * gamma[i] / dt;
		auto term2 = rho * Vector2D(midPoint.y * term1, -midPoint.x * term1);
		f += term2;
	}
	f = f * -0.5;
	return f;
}

//index:bubble的索引
Vector2D FoamVortexSolver::computeTwoWayForce(int index, double dt) {
	auto& bubble_panelset = _foamVortexData->bubble_panelset;
	int len = bubble_panelset[index]->size();
	double rho = 1.0;
	double c = 0.2;
	auto& gammas = _foamVortexData->bubble_slip_strength;
	Vector2D f;
	for (int i = 0; i < len; ++i) {
		auto midPoint = bubble_panelset[index]->midPoint(i);
		auto term1 = c * gammas[index][i] / dt;
		auto term2 = rho * Vector2D(midPoint.y * term1, -midPoint.x * term1);
		f += term2;
	}
	f = f * -0.5;
	return f;
}


void FoamVortexSolver::generatePanelSet(const Array<Vector2D>& pos,
	const Array<double>& radius) {

	int n = pos.dataSize();
	for (int i = 0; i < n; ++i) {
		RegularPolygonPtr obj = std::make_shared<RegularPolygon>(10, pos.lookAt(i), radius.lookAt(i));
		_foamVortexData->bubble_panelset.push(obj);
	}
	_foamVortexData->bubble_slip_strength.reSize(n);

}



Vector2D FoamVortexSolver::computeUSingle(const Vector2D& pos, int i)const {

	auto position = _foamVortexData->vortexPosition;
	auto gamma = _foamVortexData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return gamma[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));
}

void FoamVortexSolver::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_foamVortexData->vorticities().reSize(numberOfParticles);
}

void FoamVortexSolver::setPanels(RegularPolygonPtr surfaces) {
	_foamVortexData->panelSet = surfaces;
	computeBoundaryMatrix();
}



void FoamVortexSolver::emitVortexRing() {
	auto data = _foamVortexData;
	auto& vel = data->vortexVelocity;
	auto& pos = data->vortexPosition;
	auto& gamma = data->vorticities();

	vel.reSize(4);

	Vector2D A(0.2, 1.2);
	Vector2D B(0.2, 1.1);
	Vector2D C(0.2, 1.0);
	Vector2D D(0.2, 0.9);

	pos.push(A);
	pos.push(B);
	pos.push(C);
	pos.push(D);

	gamma.push(0.6);
	gamma.push(0.6);
	gamma.push(-0.6);
	gamma.push(-0.6);

	/*gamma.push(random_double(0.4, 0.8));
	gamma.push(random_double(0.4, 0.8));
	gamma.push(-random_double(0.4, 0.8));
	gamma.push(-random_double(0.4, 0.8));*/

}


void FoamVortexSolver::emitParticlesFromPanels(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto& pos = data->vortexPosition;
	auto& vel = data->vortexVelocity;
	auto panels = data->panelSet;

	auto gamma = data->slip_Strength;
	int emitNum = panels->size();
	Vector2D tempPos;
	static int step = 0;

	if (step % 3 == 0) {
		for (int i = 0; i < emitNum; ++i) {

			auto line = panels->lookAt(i).end - panels->lookAt(i).start;
			double panelLength = line.getLength();
			auto lambda = random_double(0, 1);
			tempPos = panels->midPoint(i);
			tempPos += panels->lookAt(i).normal * random_double(0.01, 0.02);

			pos.push(tempPos);
			vel.push(Vector2D(0.0, 0.0));
			double temp_vorticity = 0.00;

			if (gamma.size() > 0) {
				//temp_vorticity = (vs_tau * gamma[i] * timeIntervalInSeconds * panelLength);

			}
			data->vorticities().push(temp_vorticity);
		}

	}
	step++;
}



//计算index点处的速度值
//这里的index是流体粒子的index
Vector2D FoamVortexSolver::computeSingleVelocityFromPanels(int index) {
	auto& pos = _foamVortexData->vortexPosition;
	auto panel = _foamVortexData->panelSet;
	auto panelSize = panel->size();
	auto& gama1 = _foamVortexData->no_through_strength;

	Vector2D result;

	for (int i = 0; i < panelSize; ++i) {

		auto start = panel->lookAt(i).start;
		auto end = panel->lookAt(i).end;
		auto normal = panel->lookAt(i).normal;

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



//这里的index是panel的index
Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
	auto panel = _foamVortexData->panelSet;
	auto start = panel->lookAt(index).start;
	auto end = panel->lookAt(index).end;
	auto normal = panel->lookAt(index).normal;

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

	result = vel_to_world(result, start, normal);
	return result;
}





//只执行一次
void FoamVortexSolver::computeBoundaryMatrix() {
	auto panels = _foamVortexData->panelSet;
	auto panelSize = panels->size();
	Eigen::MatrixXd& A = _foamVortexData->no_through_matrix;
	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;

	auto sizex = panelSize + 1;
	auto sizey = panelSize;
	A.resize(sizex, sizey);
	B.resize(sizex, sizey);

	for (int j = 0; j < panelSize; ++j) {
		auto normal = panels->lookAt(j).normal;
		for (int i = 0; i < panelSize; ++i) {
			auto mid_j = panels->midPoint(j);
			auto u_ji = computeUnitVelocityFromPanels(i, mid_j).dot(normal);
			A(j, i) = -u_ji;
		}
	}

	for (int i = 0; i < panelSize; ++i) {
		A(panelSize, i) = 1.0;
	}

	for (int j = 0; j < panelSize; ++j) {
		auto normal = panels->lookAt(j).normal;
		Vector2D tengent(normal.y, -normal.x);
		for (int i = 0; i < panelSize; ++i) {
			auto mid_j = panels->midPoint(j);
			auto u_ji = computeUnitVelocityFromPanels(i, mid_j).dot(tengent);
			B(j, i) = -u_ji;
		}
	}

	for (int i = 0; i < panelSize; ++i) {
		B(panelSize, i) = 1.0;
	}
}



//在这里求解vortex sheet strength
void FoamVortexSolver::vortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto panels = data->panelSet;
	auto panelSize = panels->size();
	auto& pos = data->vortexPosition;
	auto n = pos.dataSize();

	Eigen::MatrixXd& A = _foamVortexData->no_through_matrix;
	Eigen::VectorXd& x = _foamVortexData->no_through_strength;

	//组装b
	Eigen::VectorXd b(panelSize + 1);
	for (int i = 0; i < panelSize; ++i) {
		auto normal = panels->lookAt(i).normal;
		auto pos = panels->midPoint(i);
		auto vec = vs_vec;

		Vector2D temp;
		for (int j = 0; j < n; ++j) {
			temp += computeUSingle(pos, j);
		}
		b[i] = (vec + temp).dot(normal);
	}

	b[panelSize] = 0;

	x = A.colPivHouseholderQr().solve(b);

}


//在这里求解vortex sheet strength
void FoamVortexSolver::slipVortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto panels = data->panelSet;
	auto panelSize = panels->size();
	auto& pos = data->vortexPosition;
	auto n = pos.dataSize();

	auto& vorticity = data->vorticities();

	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
	Eigen::VectorXd& x1 = _foamVortexData->slip_Strength;


	Eigen::VectorXd b1(panelSize + 1);
	for (int i = 0; i < panelSize; ++i) {
		auto normal = panels->lookAt(i).normal;
		Vector2D tengent(normal.y, -normal.x);

		auto pos = panels->midPoint(i);
		auto vec = vs_vec;

		Vector2D temp;
		for (int j = 0; j < n; ++j) {
			temp += computeUSingle(pos, j);
		}
		b1[i] = (vec + temp).dot(tengent);
	}

	b1[panelSize] = 0;

	x1 = B.colPivHouseholderQr().solve(b1);

	auto line = panels->lookAt(0).end - panels->lookAt(0).start;
	double panelLength = line.getLength();

	for (int i = 0; i < panelSize; ++i) {
		vorticity[n - i - 1] = (vs_tau * x1[panelSize - i - 1] * timeIntervalInSeconds * panelLength);
	}
}



//求解two-way下的vortex strength
void FoamVortexSolver::movingSlipVortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto panels = data->panelSet;
	auto panelSize = panels->size();
	auto& pos = data->vortexPosition;
	auto n = pos.dataSize();

	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
	Eigen::VectorXd& x1 = _foamVortexData->slip_Strength;
	Eigen::VectorXd b1(panelSize + 1);

	for (int i = 0; i < panelSize; ++i) {
		auto normal = panels->lookAt(i).normal;
		Vector2D tengent(normal.y, -normal.x);

		auto pos = panels->midPoint(i);
		//auto vec = vs_vec;
		auto vec = Vector2D::zero();
		Vector2D temp;
		for (int j = 0; j < n; ++j) {
			temp += computeUSingle(pos, j);
		}
		b1[i] = (vec + temp).dot(tengent);
	}

	b1[panelSize] = 0;
	x1 = B.colPivHouseholderQr().solve(b1);

}


//求解two-way下的vortex strength
//求解整个bubble团上的vortex strength
void FoamVortexSolver::all_movingSlipVortexSheetSolve(double dt) {
	auto& bubble_panelSet = _foamVortexData->bubble_panelset;
	int bubble_num = bubble_panelSet.dataSize();
	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
	auto& pos = _foamVortexData->vortexPosition;
	auto n = pos.dataSize();
	auto& vorticity = _foamVortexData->vorticities();

	//遍历所有的bubble_panelset
	for (int i = 0; i < bubble_num; ++i) {
		auto panels = bubble_panelSet[i];
		auto panelSize = panels->size();
		Eigen::VectorXd& x1 = _foamVortexData->bubble_slip_strength[i];
		Eigen::VectorXd b1(panelSize + 1);

		for (int j = 0; j < panelSize; ++j) {
			auto normal = panels->lookAt(j).normal;
			Vector2D tangent(normal.y, -normal.x);
			auto mid_pos = panels->midPoint(j);
			Vector2D temp;
			for (int k = 0; k < n; ++k) {
				temp += computeUSingle(mid_pos, k);

			}
			b1[j] = temp.dot(tangent);
		}

		b1[panelSize] = 0;
		x1 = B.colPivHouseholderQr().solve(b1);
	}

}

