#include "VortexParticleSystemSolver2.h"

#include "../ConstVar.h"

#include "../random.h"

#include "../Matrix3x3.hpp"

#include <iostream>

const double eps = 0.00001;

const Vector2D vs_vec = Vector2D(0.01, 0.0);

//newPosition赋值给positions可以改用移动构造函数，提高效率

VortexParticleSystemSolver2::VortexParticleSystemSolver2() {
	_particleSystemData = make_shared<VortexParticleSystemData2>();
	_vortexParticleData = make_shared<VortexParticleSystemData2>();
	_vortexParticleData = std::dynamic_pointer_cast<VortexParticleSystemData2>(_particleSystemData);
}

VortexParticleSystemSolver2::~VortexParticleSystemSolver2() {

}

void VortexParticleSystemSolver2::timeIntegration(double timeIntervalInSeconds) {

	auto n = _vortexParticleData->numberOfParticles();
	auto positions = _vortexParticleData->positions();
	auto forces = _vortexParticleData->forces();

	//计算重力
	for (int i = 0; i < n; ++i) {
		//_newVelocities[i] += GRAVITY * timeIntervalInSeconds * -0.7;
		//cout << _newVelocities[0].x << endl;
	}


	for (int i = 0; i < n; ++i) {
		/*for (int j = 0; j < n; ++j) {
			if (i != j) {
				_newVelocities[i] += computeUSingle(positions[i], j);
			}
		}*/

		_newPositions[i] = positions[i] + timeIntervalInSeconds * _newVelocities[i];
	}


}

void VortexParticleSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//做vortex sheet boundary的时候要把这个onBeginAdvanceTimeStep注释掉
	//onBeginAdvanceTimeStep();
	timeIntegration(timeIntervalInSeconds);
	//ParticleSystemSolver2::resolveCollision();
	onEndAdvanceTimeStep();
}


VortexParticleSystemData2Ptr& VortexParticleSystemSolver2::vortexParticleData() {
	return _vortexParticleData;
}


void VortexParticleSystemSolver2::onBeginAdvanceTimeStep() {
	ParticleSystemSolver2::beginAdvanceTimeStep();
}

void VortexParticleSystemSolver2::onEndAdvanceTimeStep() {
	ParticleSystemSolver2::endAdvanceTimeStep();
}


Vector2D VortexParticleSystemSolver2::computeUSingle(const Vector2D& pos, int i)const {

	auto position = _vortexParticleData->positions();
	auto vorticity = _vortexParticleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (eps * eps)));
}

void VortexParticleSystemSolver2::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_vortexParticleData->vorticities().reSize(numberOfParticles);

	auto vorticity = _vortexParticleData->vorticities();

	//为了方便测试，给每个粒子赋一个随机的涡量
	for (int i = 0; i < numberOfParticles; ++i) {
		//vorticity[i] = random_double(-0.01, 0.01);
		vorticity[i] = 0.000;
	}


}

void VortexParticleSystemSolver2::setPanels(ExplicitSurface2Ptr surfaces) {
	_vortexParticleData->panelSet = surfaces;
	correctPanelCoordinateSystem();
}

void VortexParticleSystemSolver2::emitParticlesFromPanel() {
	auto data = _vortexParticleData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto& pos = data->positions();
	//遍历每个panel，在panel上随机生成粒子
	auto panelNum = panels->size();

	for (auto i = 0; i < panelNum; ++i) {
		const auto p = panels->lookAt(i);
		auto midPoint = 0.5 * (p.start + p.end);

		//在边界附近随机生成粒子

		auto line = (p.end - p.start).getNormalize();
		auto panelLength = (p.end - p.start).getLength();

		int emitNum = 3;
		for (int j = 0; j < emitNum; ++j) {

			auto lambda = random_double(0.0, panelLength);
			auto ppos = p.start + lambda * line;

			ppos += random_double(0.0, 0.03) * p.normal;

			pos.push(ppos);
			data->vorticities().push(0.0);
			_newVelocities.push(vs_vec);
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}

	}
}


Vector2D VortexParticleSystemSolver2::computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
	auto panel = _vortexParticleData->panelSet;
	auto start = panel->lookAt(index).start;
	auto end = panel->lookAt(index).end;
	auto normal = panel->lookAt(index).normal;

	double eps = 0.01;

	//首先，组装坐标变换矩阵
	auto transToLocal = Matrix3x3<double>::transToLocalMatrix(normal, start);
	auto transToWorld = transToLocal.inverse();

	Vector2D result;

	//计算beta值.
	double beta = 0.0;
	auto vec_r1 = start - midPoint;
	auto vec_r2 = end - midPoint;
	auto r1 = vec_r1.getLength();
	auto r2 = vec_r2.getLength();
	beta = acos(vec_r1.dot(vec_r2) / (r1 * r2));

	//eq(16) eq(17)
	//按照论文上的逻辑来看，这里算出来的是局部坐标系下的表示
	result.x = beta / (2 * kPiD);
	result.y = log10((r2 + eps) / (r1 + eps)) / (2 * kPiD);

	result = transToWorld * result;
	return result;
}

//要确保normal x (end - start) < 0
void VortexParticleSystemSolver2::correctPanelCoordinateSystem() {
	auto panels = _vortexParticleData->panelSet;
	auto size = panels->size();

	for (int i = 0; i < size; ++i) {
		auto& data = (*panels)[i];
		if (data.normal.cross(data.end - data.start) < 0) {
			std::swap(data.end, data.start);
		}
	}
}


void VortexParticleSystemSolver2::computeBoundaryMatrix(Eigen::Matrix3f& A) {
	auto panels = _vortexParticleData->panelSet;
	auto panleSize = panels->size();
	for (int i = 0; i < panleSize; ++i) {

		for (int j = 0; j < panleSize; ++j) {
			auto mid_j = panels->midPoint(j);
		}
	}
}

//在这里求解vortex sheet strength
void VortexParticleSystemSolver2::vortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _vortexParticleData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto& pos = data->positions();


}