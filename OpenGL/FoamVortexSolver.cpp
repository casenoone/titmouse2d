//#include "FoamVortexSolver.h"
//
//#include "../titmouse2d/src/ConstVar.h"
//
//#include "../titmouse2d/src/random.h"
//
//#include "../titmouse2d/src/Matrix3x3.hpp"
//
//#include "../titmouse2d/src/boundingbox2.h"
//
//#include "../titmouse2d/src/LinearSystem/JacobiSolver.hpp"
//
//#include "../titmouse2d/src/LinearSystem/ConjugateGradientSolver.hpp"
//#include <omp.h>
//
//#include <iostream>
//#include <cmath>
//
//
//static const int numOfStep = 1;
//static const double vorticity_eps = 0.08;
//static double fv_eps = 0.001;
//static int static_boudary_interval = 20;
//
//static const double rad_k = 0.012;
//
//Vector2D vel_to_world(const Vector2D vel_local, const Vector2D n_i, const Vector2D normal_i) {
//	double cos_theta = normal_i.y; // cos(theta)
//	double sin_theta = normal_i.x; // sin(theta)
//	Vector2D vel = Vector2D(cos_theta * vel_local.x + sin_theta * vel_local.y,
//		-sin_theta * vel_local.x + cos_theta * vel_local.y);
//	return vel;
//}
//
//FoamVortexSolver::FoamVortexSolver() {
//	_particleSystemData = std::make_shared<FoamVortexData>();
//	_foamVortexData = std::make_shared<FoamVortexData>();
//	_foamVortexData = std::dynamic_pointer_cast<FoamVortexData>(_particleSystemData);
//}
//
//FoamVortexSolver::FoamVortexSolver(
//	const Vector2I& resolution,
//	const Vector2D& gridSpacing,
//	const Vector2D& gridOrigin) :FoamVortexSolver() {
//
//	_shallowWaveSolver = std::make_shared<ShallowWaveSolver2>(resolution, gridSpacing, gridOrigin);
//}
//
//
//
//void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {
//
//	auto& vortex_pos = _foamVortexData->vortexPosition;
//	auto& vortex_vel = _foamVortexData->vortexVelocity;
//	auto n = vortex_pos.dataSize();
//	Array<Vector2D> tempP(n);
//
//	for (int i = 0; i < n; ++i) {
//		Vector2D vortexVel;
//		for (int j = 0; j < n; ++j) {
//			if (i != j) {
//				vortexVel += computeUSingle(vortex_pos[i], j);
//			}
//		}
//		Vector2D votexSheetVel;
//		votexSheetVel = static_computeSingleVelocityFromPanels(i);
//
//		tempP[i] = vortex_pos[i] + timeIntervalInSeconds * (votexSheetVel + vortexVel);
//	}
//	//这里搞一个移动赋值函数
//	vortex_pos = tempP;
//
//
//	//求解tracer粒子的速度场
//	tracerParticlesSolve();
//
//
//	//在这里更新tracer粒子
//	auto tracerPos = _foamVortexData->tracePosition;
//	auto traceVel = _foamVortexData->traceVelocity;
//	auto tracer_n = tracerPos.dataSize();
//	for (int i = 0; i < tracer_n; ++i) {
//		tracerPos[i] += (traceVel[i]) * timeIntervalInSeconds;
//		tarcerCollisionSolve(tracerPos[i]);
//	}
//
//
//	//气泡的时间积分
//	int bubble_num = _foamVortexData->numberOfParticles();
//	auto& force = _foamVortexData->forces();
//	auto& bubble_pos = _foamVortexData->positions();
//	auto& bubble_vel = _foamVortexData->velocities();
//	int vortex_num = _foamVortexData->vortexPosition.dataSize();
//
//	for (int i = 0; i < bubble_num; ++i) {
//		auto& newVelocity = _newVelocities[i];
//		Vector2D induce_v;
//		for (int j = 0; j < vortex_num; ++j) {
//			induce_v += computeUSingle(bubble_pos[i], j);
//		}
//
//		newVelocity = bubble_vel[i] + (force[i] / 1) * timeIntervalInSeconds;
//		auto& newPosition = _newPositions[i];
//		auto temp2 = bubble_pos[i] + (newVelocity + induce_v) * timeIntervalInSeconds;
//		newPosition = temp2;
//		tarcerCollisionSolve(newPosition);
//	}
//
//}
//
//
//void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {
//	_foamVortexData->neighbor->setNeiborList(0.12, _foamVortexData->positions());
//	beginAdvanceTimeStep();
//	computeTotalForce(timeIntervalInSeconds);
//	//no_through_solve(timeIntervalInSeconds);
//	timeIntegration(timeIntervalInSeconds);
//	emitParticlesFromPanels(timeIntervalInSeconds);
//	all_bubble_vortexStrengthSolve(timeIntervalInSeconds);
//
//	update_bubble_panelset_pos(timeIntervalInSeconds);
//
//	decayVorticity();
//	//_shallowWaveSolver->onAdvanceTimeStep(timeIntervalInSeconds);
//
//	endAdvanceTimeStep();
//	//bubbleBreakUp();
//
//}
//
//
//FoamVortexDataPtr& FoamVortexSolver::foamVortexData() {
//	return _foamVortexData;
//}
//
//
//Vector2D FoamVortexSolver::computeUSingle(const Vector2D& pos, int i)const {
//
//	auto position = _foamVortexData->vortexPosition;
//	auto gamma = _foamVortexData->gamma();
//	auto r2 = (pos - position[i]).getLengthSquared();
//	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
//	return gamma[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));
//
//}
//
//
//
//void FoamVortexSolver::setMovingBoudnary(RegularPolygonPtr surfaces) {
//	_foamVortexData->panelSet = surfaces;
//	constructMovingBoundaryMatrix();
//}
//
//void FoamVortexSolver::setStaticBoudnary(ExplicitSurface2Ptr surfaces) {
//	auto box = surfaces;
//	auto& staticBoundary = _foamVortexData->staticBoundary;
//	int num = 0;
//	for (int i = 0; i < 4; ++i) {
//		auto start = box->lookAt(i).end;
//		auto end = box->lookAt(i).start;
//		auto normal = box->lookAt(i).normal;
//
//		auto line = end - start;
//		auto length = line.getLength();
//		line = line.getNormalize();
//
//		double interval = length / (static_boudary_interval);
//		for (int j = 0; j < static_boudary_interval; ++j) {
//			auto temp_start = start + j * interval * line;
//			auto temp_end = start + (j + 1.0) * interval * line;
//			FoamVortexData::Panel panel(temp_start, temp_end, 0.5 * (temp_start + temp_end), normal);
//			staticBoundary.push_back(panel);
//		}
//
//	}
//
//	constructStaticBoundaryMatrix();
//}
//
//
//void FoamVortexSolver::setShallowWaveMovingBoundary(const Vector2D& center, const double r) {
//	_shallowWaveSolver->setSphereMarkers(center, r);
//}
//
////index:bubble的索引
//Vector2D FoamVortexSolver::computeTwoWayForce(int index, double dt) {
//	auto& bubble_panelset = _foamVortexData->bubble_panelset;
//	int len = bubble_panelset[index]->size();
//
//	double rho = 1.0;
//	double c = 0.6;
//	auto& gammas = _foamVortexData->bubble_slip_strength;
//	Vector2D f;
//	for (int i = 0; i < len; ++i) {
//		auto midPoint = bubble_panelset[index]->midPoint(i);
//		double term1 = 0;
//		if (gammas[index].size() != 0)
//			term1 = c * gammas[index][i] / dt;
//		auto term2 = rho * Vector2D(midPoint.y * term1, -midPoint.x * term1);
//		f += term2;
//	}
//	f = f * -0.5;
//	return f;
//}
//
//void FoamVortexSolver::generatePanelSet(const Array<Vector2D>& pos,
//	const Array<double>& radius) {
//
//	int n = pos.dataSize();
//	for (int i = 0; i < n; ++i) {
//		RegularPolygonPtr obj = std::make_shared<RegularPolygon>(10, pos.lookAt(i), radius.lookAt(i));
//		_foamVortexData->bubble_panelset.push(obj);
//	}
//	_foamVortexData->bubble_slip_strength.reSize(n);
//
//	constructTwoWayBoundaryMatrix();
//
//}
//
//
//
//void FoamVortexSolver::emitVortexRing() {
//	auto& pos = _foamVortexData->vortexPosition;
//	auto& vel = _foamVortexData->vortexVelocity;
//	auto& gamma = _foamVortexData->gamma();
//	int n = 4;
//	pos.reSize(n);
//	vel.reSize(n);
//	gamma.reSize(n);
//	Vector2D A(0.2, 1.2);
//	Vector2D B(0.2, 1.1);
//	Vector2D C(0.2, 1.0);
//	Vector2D D(0.2, 0.9);
//
//	pos[0] = A;
//	pos[1] = B;
//	pos[2] = C;
//	pos[3] = D;
//
//	gamma[0] = 0.6;
//	gamma[1] = 0.6;
//	gamma[2] = -0.6;
//	gamma[3] = -0.6;
//}
//
//
//void FoamVortexSolver::emitParticlesFromPanels(double timeIntervalInSeconds) {
//
//	static int step = 0;
//	auto data = _foamVortexData;
//	auto& pos = _foamVortexData->vortexPosition;
//	auto panelVel = data->panelSet->velocity;
//	auto panel = data->panelSet;
//	auto& emitParticle = data->newParticles;
//
//	auto emitNum = emitParticle.dataSize();
//
//	//更改粒子发射位置使得与panel同步
//	for (int i = 0; i < emitNum; ++i) {
//		emitParticle[i] += panelVel * timeIntervalInSeconds;
//	}
//
//	if (step % 4 == 0 && pos.dataSize() < 100000 && (panel->center().x - panel->r()) < 2.0) {
//		auto& pos = data->vortexPosition;
//		auto& vel = data->vortexVelocity;
//		auto panels = data->panelSet;
//		auto gamma = data->slip_strength;
//
//		Vector2D tempPos;
//
//		auto startNum = pos.dataSize();
//
//		//push_back这里可作优化
//		for (int i = 0; i < emitNum; ++i) {
//			pos.push(emitParticle(i));
//			vel.push(Vector2D(0.0, 0.0));
//			double temp_gamma = 0;
//			data->gamma().push(temp_gamma);
//		}
//
//
//		Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
//		Eigen::VectorXd& x = _foamVortexData->slip_strength;
//
//		auto panelSize = data->panelSet->size();
//		auto panelVelocity = data->panelSet->velocity;
//		//组装b
//		Eigen::VectorXd b(panelSize * 2);
//
//		for (int i = 0; i < panelSize; ++i) {
//			auto temp_pos = panels->midPoint(i);
//			Vector2D tempVel;
//			for (int j = 0; j < pos.dataSize(); ++j) {
//				tempVel += computeUSingle(temp_pos, j);
//			}
//			auto v1 = (panelVelocity - tempVel);
//			b[i] = v1.x;
//			b[i + panelSize] = v1.y;
//		}
//
//		x = B * b;
//		auto& vor = data->gamma();
//		int j = 0;
//		for (int i = startNum; i < pos.dataSize(); ++i) {
//			vor[i] = x[j++];
//		}
//	}
//
//	step++;
//}
//
//
//
//
////计算在pos处引发的诱导速度
//Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(const Vector2D& pos, int index) {
//
//	auto position = _foamVortexData->newParticles;
//	auto r2 = (pos - position[index]).getLengthSquared();
//	auto uv = Vector2D(position[index].y - pos.y, pos.x - position[index].x);
//	return uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (vorticity_eps * vorticity_eps)));
//}
//
//
////移动边界所使用的tracer粒子
//void FoamVortexSolver::emitTracerParticles() {
//	static int step = 0;
//	auto data = _foamVortexData;
//	auto& tracerPos = _foamVortexData->tracePosition;
//	auto& tracerVel = _foamVortexData->traceVelocity;
//	auto n = tracerPos.dataSize();
//	auto panels = data->panelSet;
//
//	int emitNum = 0;
//	//int emitNum = 1;
//	tracerPos.reSize(emitNum);
//	tracerVel.reSize(emitNum);
//	Vector2D tempPos;
//	for (int i = 0; i < emitNum; ++i) {
//		tempPos.x = random_double(0.2, 1.0);
//		tempPos.y = random_double(0.5, 1.5);
//		tracerPos[i] = tempPos;
//	}
//}
//
//
////求解tracer粒子的运动
//void FoamVortexSolver::tracerParticlesSolve() {
//	auto& tracerPos = _foamVortexData->tracePosition;
//	auto& tracerVel = _foamVortexData->traceVelocity;
//	auto n = tracerPos.dataSize();
//
//	auto vor_n = _foamVortexData->vortexPosition.dataSize();
//	for (int i = 0; i < n; ++i) {
//		Vector2D tempVel;
//		Vector2D pos = tracerPos(i);
//		for (int j = 0; j < vor_n; ++j) {
//			tempVel += computeUSingle(pos, j);
//		}
//		tracerVel(i) = tempVel;
//	}
//}
//
//
////这里的index是panel的index
//Vector2D FoamVortexSolver::static_computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
//	auto& panel = _foamVortexData->staticBoundary;
//	auto start = panel[index].start;
//	auto end = panel[index].end;
//	auto normal = panel[index].normal;
//
//	Vector2D result;
//
//	//计算beta值.
//	double beta = 0.0;
//	auto vec_r1 = start - midPoint;
//	auto vec_r2 = end - midPoint;
//	auto r1 = vec_r1.getLength();
//	auto r2 = vec_r2.getLength();
//	auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
//	beta = acos(temp1);
//	if (isnan(beta)) {
//		beta = kPiD;
//	}
//	//eq(16) eq(17)
//	result.x = beta / (2.0 * kPiD);
//	result.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);
//
//	result = vel_to_world(result, start, normal);
//	return result;
//}
//
//
////two-way
//Vector2D FoamVortexSolver::two_way_computeUnitVelocityFromPanels(int bubble_idx, int panelIdx, const Vector2D& midPoint) {
//	auto& panel = _foamVortexData->bubble_panelset[bubble_idx];
//	auto start = panel->lookAt(panelIdx).start;
//	auto end = panel->lookAt(panelIdx).end;
//	auto normal = panel->lookAt(panelIdx).normal;
//
//	Vector2D result;
//
//	//计算beta值.
//	double beta = 0.0;
//	auto vec_r1 = start - midPoint;
//	auto vec_r2 = end - midPoint;
//	auto r1 = vec_r1.getLength();
//	auto r2 = vec_r2.getLength();
//	auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
//	beta = acos(temp1);
//	if (isnan(beta)) {
//		beta = kPiD;
//	}
//	//eq(16) eq(17)
//	result.x = beta / (2.0 * kPiD);
//	result.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);
//
//	result = vel_to_world(result, start, normal);
//	return result;
//}
//
//Vector2D FoamVortexSolver::static_computeSingleVelocityFromPanels(int index) {
//	auto& pos = _foamVortexData->vortexPosition;
//	auto& panel = _foamVortexData->staticBoundary;
//	auto panelSize = panel.size();
//	auto& gama1 = _foamVortexData->no_through_strength;
//
//	Vector2D result;
//
//	for (int i = 0; i < panelSize; ++i) {
//
//		auto start = panel[i].start;
//		auto end = panel[i].end;
//		auto normal = panel[i].normal;
//
//		Vector2D temp2;
//
//		double beta = 0.0;
//		auto vec_r1 = start - pos[index];
//		auto vec_r2 = end - pos[index];
//		auto r1 = vec_r1.getLength();
//		auto r2 = vec_r2.getLength();
//		auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
//		beta = acos(temp1);
//		if (isnan(beta)) {
//			beta = kPiD;
//		}
//
//		temp2.x = beta / (2.0 * kPiD);
//		temp2.y = log((r2 + fv_eps) / (r1 + fv_eps)) / (2.0 * kPiD);
//
//		double tempgamma = 0.0;
//		if (gama1.size() > 0)
//			tempgamma = gama1[i];
//		temp2 = temp2 * tempgamma;
//		temp2 = vel_to_world(temp2, start, normal);
//		result += temp2;
//	}
//
//	return result;
//}
//
//
//
//void FoamVortexSolver::no_through_solve(double timeIntervalInSeconds) {
//	auto data = _foamVortexData;
//	auto& pos = data->vortexPosition;
//
//	auto n = pos.dataSize();
//	auto& panels = data->staticBoundary;
//	auto panelSize = panels.size();
//	Eigen::MatrixXd& A = _foamVortexData->no_through_matrix;
//	Eigen::VectorXd& x = _foamVortexData->no_through_strength;
//	//组装b
//	Eigen::VectorXd b(panelSize + 1);
//	for (int i = 0; i < panelSize; ++i) {
//		auto normal = panels[i].normal;
//		auto pos = panels[i].mid;
//
//		Vector2D temp;
//		for (int j = 0; j < n; ++j) {
//			temp += computeUSingle(pos, j);
//		}
//		b[i] = temp.dot(normal);
//	}
//
//	b[panelSize] = 0;
//	x = A.colPivHouseholderQr().solve(b);
//}
//
//
//
//void FoamVortexSolver::constructStaticBoundaryMatrix() {
//
//	auto& panel = _foamVortexData->staticBoundary;
//	auto num = panel.size();
//	Eigen::MatrixXd& mat = _foamVortexData->no_through_matrix;
//	auto sizex = num + 1;
//	auto sizey = num;
//	mat.resize(sizex, sizey);
//	for (int j = 0; j < num; ++j) {
//		auto normal = panel[j].normal;
//		auto pos = panel[j].mid;
//		for (int i = 0; i < num; ++i) {
//			auto u_ji = static_computeUnitVelocityFromPanels(i, pos).dot(normal);
//			mat(j, i) = -u_ji;
//		}
//	}
//
//	for (int i = 0; i < num; ++i) {
//		mat(num, i) = 1.0;
//	}
//}
//
//
//void FoamVortexSolver::constructTwoWayBoundaryMatrix() {
//	auto& bubble_panelSet = _foamVortexData->bubble_panelset;
//	int bubble_num = bubble_panelSet.dataSize();
//
//
//	auto& panels = bubble_panelSet[0];
//	auto panelSize = panels->size();
//	Eigen::MatrixXd& B = _foamVortexData->bubble_slip_matrix;
//
//	auto sizex = panelSize + 1;
//	auto sizey = panelSize;
//	B.resize(sizex, sizey);
//
//	for (int j = 0; j < panelSize; ++j) {
//		auto normal = panels->lookAt(j).normal;
//		Vector2D tengent(normal.y, -normal.x);
//		for (int i = 0; i < panelSize; ++i) {
//			auto mid_j = panels->midPoint(j);
//			auto u_ji = two_way_computeUnitVelocityFromPanels(0, i, mid_j).dot(tengent);
//			B(j, i) = -u_ji;
//		}
//	}
//
//	for (int i = 0; i < panelSize; ++i) {
//		B(panelSize, i) = 1.0;
//	}
//}
//
//
//void FoamVortexSolver::all_bubble_vortexStrengthSolve(double dt) {
//	auto& bubble_panelSet = _foamVortexData->bubble_panelset;
//	int bubble_num = bubble_panelSet.dataSize();
//	Eigen::MatrixXd& B = _foamVortexData->bubble_slip_matrix;
//	auto& pos = _foamVortexData->vortexPosition;
//	auto n = pos.dataSize();
//	auto& vorticity = _foamVortexData->gamma();
//
//	//遍历所有的bubble_panelset
//	for (int i = 0; i < bubble_num; ++i) {
//		auto panels = bubble_panelSet[i];
//		auto panelSize = panels->size();
//		Eigen::VectorXd& x1 = _foamVortexData->bubble_slip_strength[i];
//		Eigen::VectorXd b1(panelSize + 1);
//
//		for (int j = 0; j < panelSize; ++j) {
//			auto normal = panels->lookAt(j).normal;
//			Vector2D tangent(normal.y, -normal.x);
//			auto mid_pos = panels->midPoint(j);
//			Vector2D temp;
//			for (int k = 0; k < n; ++k) {
//				temp += computeUSingle(mid_pos, k);
//
//			}
//			b1[j] = temp.dot(tangent);
//		}
//
//		b1[panelSize] = 0;
//		x1 = B.colPivHouseholderQr().solve(b1);
//	}
//}
//
//
//
//
////只执行一次
//void FoamVortexSolver::constructMovingBoundaryMatrix() {
//	auto panels = _foamVortexData->panelSet;
//	auto& emittedParticles = _foamVortexData->newParticles;
//	auto panelSize = panels->size();
//	auto emitNum = panelSize * numOfStep;
//	emittedParticles.reSize(emitNum);
//
//	Vector2D tempPos;
//	for (int i = 0; i < panelSize; ++i) {
//		for (int j = 0; j < numOfStep; ++j) {
//			auto line = panels->lookAt(i).end - panels->lookAt(i).start;
//			auto lambda = random_double(0, 1);
//			tempPos = panels->lookAt(i).start + lambda * line;
//			tempPos += panels->lookAt(i).normal * random_double(0.01, 0.02);
//			emittedParticles(i * numOfStep + j) = tempPos;
//		}
//	}
//
//	Eigen::MatrixXd A;
//
//	A.resize(panelSize * 2, emitNum);
//	for (int i = 0; i < panelSize; ++i) {
//		auto pos = panels->midPoint(i);
//		for (int j = 0; j < emitNum; ++j) {
//			auto vel = computeUnitVelocityFromPanels(pos, j);
//			A(i, j) = vel.x;
//			A(i + panelSize, j) = vel.y;
//		}
//	}
//
//
//
//	Eigen::MatrixXd& B = _foamVortexData->slip_matrix;
//	Eigen::MatrixXd A_trans = A.transpose();
//
//	auto I = Eigen::MatrixXd::Identity(emitNum, emitNum);
//	B = (A_trans * A + 3 * I).inverse() * A_trans;
//}
//
//void FoamVortexSolver::tarcerCollisionSolve(Vector2D& pos) {
//	if (pos.x > 2)
//		pos.x = 2;
//	if (pos.x < 0)
//		pos.x = 0;
//	if (pos.y > 2)
//		pos.y = 2;
//	if (pos.y < 0)
//		pos.y = 0;
//}
//
//
//
//void FoamVortexSolver::decayVorticity() {
//	auto& vorticity = _foamVortexData->gamma();
//	auto len = vorticity.dataSize();
//	for (int i = 0; i < len; ++i) {
//		static double init = vorticity[i];
//		if (vorticity[i] >= init * 0.2)
//			vorticity[i] = vorticity[i] - vorticity[i] * 0.008;
//	}
//}
//
//
//void FoamVortexSolver::update_bubble_panelset_pos(double dt) {
//	auto& bubble_panelset = _foamVortexData->bubble_panelset;
//	auto bubble_panelset_n = bubble_panelset.dataSize();
//	auto& force = _foamVortexData->forces();
//	for (int i = 0; i < bubble_panelset_n; ++i) {
//		auto currentPanels = bubble_panelset[i];
//		currentPanels->updateVelocity(dt, force[i]);
//		currentPanels->updatePosition(dt);
//	}
//	//std::cout << 888 << std::endl;
//}
//
//
//Vector2D FoamVortexSolver::computeF_rB(int i, int j) const {
//	auto pos = _foamVortexData->positions();
//	auto& radius = _foamVortexData->particleRadius;
//
//	auto p_ij = pos[i] - pos[j];
//	auto rad_ij = radius[i] + radius[j];
//	auto p_ij_norm2 = p_ij.getLength();
//
//	return _foamVortexData->kr * (1 / (p_ij_norm2)-(1 / rad_ij)) * p_ij;
//}
//
//
//Vector2D FoamVortexSolver::computeF_aB(int i, int j) const {
//	auto pos = _foamVortexData->positions();
//	auto& radius = _foamVortexData->particleRadius;
//	auto& neighbor = _foamVortexData->neighbor->neighBors();
//
//	int NB_i = 0;
//	int NB_j = 0;
//
//	auto& p_i = pos[i];
//	auto& p_j = pos[j];
//	auto p_ji = pos[j] - pos[i];
//	auto p_ij_norm2 = (p_i - p_j).getLength();
//
//	for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); iter++) {
//		auto neighbor_index = *iter;
//		auto rad_each = radius[i] + radius[neighbor_index];
//		auto dis = pos[i].dis(pos[neighbor_index]);
//		if (dis <= rad_each + rad_k && i != neighbor_index) {
//			NB_i++;
//		}
//	}
//
//	for (auto iter = neighbor[j].begin(); iter != neighbor[j].end(); iter++) {
//		auto neighbor_index = *iter;
//		auto rad_each = radius[j] + radius[neighbor_index];
//		auto dis = pos[j].dis(pos[neighbor_index]);
//		if (dis <= rad_each + rad_k && j != neighbor_index) {
//			NB_j++;
//		}
//	}
//
//	double inv_NBi = 0;
//	double inv_NBj = 0;
//	if (NB_i > 0)
//		inv_NBi = 1 / NB_i;
//	if (NB_j > 0)
//		inv_NBj = 1 / NB_j;
//
//	double c_nb = 0.5 * (inv_NBi + inv_NBj);
//	double c_dist = (p_ij_norm2 - std::max(radius[i], radius[j]))
//		/ std::min(radius[i], radius[j]);
//
//	return _foamVortexData->ka * c_nb * c_dist * (p_ji / p_ij_norm2);
//
//}
//
//void FoamVortexSolver::computeF_a0() {
//
//}
//
////暂时先不计算与固体的力
//void FoamVortexSolver::computeF_ra() {
//	auto pos = _foamVortexData->positions();
//	auto n = _foamVortexData->numberOfParticles();
//	auto& forces = _foamVortexData->forces();
//	auto& neighbor = _foamVortexData->neighbor->neighBors();
//	auto& radius = _foamVortexData->particleRadius;
//
//	for (int i = 0; i < n; ++i) {
//		Vector2D temp_f_r;
//		Vector2D temp_f_a;
//		for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); iter++) {
//			auto neighbor_index = *iter;
//			auto rad_each = radius[i] + radius[neighbor_index];
//			auto dis = pos[i].dis(pos[neighbor_index]);
//			if (dis <= rad_each + rad_k && i != neighbor_index) {
//				temp_f_r += computeF_rB(i, neighbor_index);
//				temp_f_a += computeF_aB(i, neighbor_index);
//			}
//		}
//
//		forces[i] += (temp_f_r + temp_f_a);
//	}
//}
//
//Vector2D FoamVortexSolver::computeF_air(int i) {
//	auto vel = _foamVortexData->velocities();
//	return -_foamVortexData->kair * vel[i];
//}
//
////这里暂时不累加fv还有与障碍物的力
//void FoamVortexSolver::computeF_fr() {
//	auto n = _foamVortexData->numberOfParticles();
//	auto& forces = _foamVortexData->forces();
//
//	for (int i = 0; i < n; ++i) {
//		forces[i] += computeF_air(i);
//	}
//}
//
//void FoamVortexSolver::compute_all_twoway_force(double dt) {
//	auto n = _foamVortexData->numberOfParticles();
//	auto& forces = _foamVortexData->forces();
//	for (int i = 0; i < n; ++i) {
//		forces[i] += computeTwoWayForce(i, dt);
//	}
//}
//
//
//void FoamVortexSolver::computeTotalForce(double dt) {
//
//	computeF_ra();
//	computeF_fr();
//	compute_all_twoway_force(dt);
//}
//
//void FoamVortexSolver::bubbleBreakUp() {
//	static int step = 0;
//	if (_foamVortexData->numberOfParticles() >= 2) {
//
//		auto& pos = _foamVortexData->positions();
//		auto& vel = _foamVortexData->velocities();
//		auto& forces = _foamVortexData->forces();
//		auto& radius = _foamVortexData->particleRadius;
//		if (step % 50 == 0) {
//			pos.pop_back();
//			vel.pop_back();
//			forces.pop_back();
//			radius.pop_back();
//			_newPositions.pop_back();
//			_newVelocities.pop_back();
//			_foamVortexData->numberOfParticles()--;
//		}
//		step++;
//
//	}
//}
