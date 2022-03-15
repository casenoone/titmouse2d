#include "FoamVortexSolver.h"

#include "../titmouse2d/src/ConstVar.h"

#include "../titmouse2d/src/random.h"

#include "../titmouse2d/src/Matrix3x3.hpp"

#include <iostream>
#include <cmath>
const double eps = 0.00001;

const Vector2D vs_vec = Vector2D(0.1, 0.0);

double vs_eps = 0.01;

//newPosition��ֵ��positions���Ը����ƶ����캯�������Ч��

FoamVortexSolver::FoamVortexSolver() {
	_particleSystemData = make_shared<FoamVortexData>();
	_foamVortexData = make_shared<FoamVortexData>();
	_foamVortexData = std::dynamic_pointer_cast<FoamVortexData>(_particleSystemData);
}

FoamVortexSolver::~FoamVortexSolver() {

}

void FoamVortexSolver::timeIntegration(double timeIntervalInSeconds) {

	auto n = _foamVortexData->numberOfParticles();
	auto positions = _foamVortexData->positions();
	auto forces = _foamVortexData->forces();


	//�������ۼ�vortex sheet���������ٶȳ�

	for (int i = 0; i < n; ++i) {
		auto vec = computeSingleVelocityFromPanels(i);
		//_newVelocities[i] = vec;
		//_newVelocities[i] += vec;
		//cout << vec.x << "," << vec.y << endl;
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

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//��vortex sheet boundary��ʱ��Ҫ�����onBeginAdvanceTimeStepע�͵�
	//onBeginAdvanceTimeStep();
	vortexSheetSolve(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	//ParticleSystemSolver2::resolveCollision();
	onEndAdvanceTimeStep();
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
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (eps * eps)));
}

void FoamVortexSolver::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {

	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	_foamVortexData->vorticities().reSize(numberOfParticles);

	auto vorticity = _foamVortexData->vorticities();

	//Ϊ�˷�����ԣ���ÿ�����Ӹ�һ�����������
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

void FoamVortexSolver::emitParticlesFromPanel() {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto& pos = data->positions();
	//����ÿ��panel����panel�������������
	auto panelNum = panels->size();

	for (auto i = 0; i < panelNum; ++i) {
		const auto p = panels->lookAt(i);
		auto midPoint = 0.5 * (p.start + p.end);

		//�ڱ߽總�������������

		auto line = (p.end - p.start).getNormalize();
		auto panelLength = (p.end - p.start).getLength();

		int emitNum = 3;
		for (int j = 0; j < emitNum; ++j) {

			auto lambda = random_double(0.0, panelLength);
			auto ppos = p.start + lambda * line;

			ppos += random_double(0.03, 0.05) * p.normal;

			pos.push(ppos);
			data->vorticities().push(0.0);
			_newVelocities.push(vs_vec);
			_newPositions.push(Vector2D());

			data->numberOfParticles()++;
		}

	}
}

Vector2D FoamVortexSolver::computeSingleVelocityFromPanels(int index) {
	auto& pos = _foamVortexData->positions();
	auto panel = _foamVortexData->panelSet;
	auto panelSize = panel->size();
	auto gama = _foamVortexData->strength;

	Vector2D result;
	for (int i = 0; i < panelSize; ++i) {
		auto start = panel->lookAt(i).start;
		auto end = panel->lookAt(i).end;
		auto normal = panel->lookAt(i).normal;

		Vector2D temp2;

		//���ȣ���װ����任����
		//����ɽ����Ż�
		auto transToLocal = Matrix3x3<double>::transToLocalMatrix(normal, start);
		auto transToWorld = transToLocal.inverse();

		double beta = 0.0;
		auto vec_r1 = start - pos[index];
		auto vec_r2 = end - pos[index];
		auto r1 = vec_r1.getLength();
		auto r2 = vec_r2.getLength();
		auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
		beta = acos(std::min(std::max(temp1, -1.0), 1.0));

		temp2.x = beta / (2 * kPiD);
		temp2.y = log10((r2 + vs_eps) / (r1 + vs_eps)) / (2 * kPiD);

		temp2 = transToWorld * temp2 * gama[i];
		result += temp2;
	}
	//cout << result.x << "," << result.y << endl;
	return result;
}

Vector2D FoamVortexSolver::computeUnitVelocityFromPanels(int index, const Vector2D& midPoint) {
	auto panel = _foamVortexData->panelSet;
	auto start = panel->lookAt(index).start;
	auto end = panel->lookAt(index).end;
	auto normal = panel->lookAt(index).normal;

	//���ȣ���װ����任����
	auto transToLocal = Matrix3x3<double>::transToLocalMatrix(normal, start);
	auto transToWorld = transToLocal.inverse();

	Vector2D result;

	//����betaֵ.
	double beta = 0.0;
	auto vec_r1 = start - midPoint;
	auto vec_r2 = end - midPoint;
	auto r1 = vec_r1.getLength();
	auto r2 = vec_r2.getLength();
	auto temp1 = vec_r1.dot(vec_r2) / (r1 * r2);
	beta = acos(std::min(std::max(temp1, -1.0), 1.0));

	//eq(16) eq(17)
	//���������ϵ��߼�������������������Ǿֲ�����ϵ�µı�ʾ
	result.x = beta / (2 * kPiD);
	result.y = log10((r2 + vs_eps) / (r1 + vs_eps)) / (2 * kPiD);

	result = transToWorld * result;

	return result;
}

//Ҫȷ��normal x (end - start) < 0
void FoamVortexSolver::correctPanelCoordinateSystem() {
	auto panels = _foamVortexData->panelSet;
	auto size = panels->size();

	for (int i = 0; i < size; ++i) {
		auto& data = (*panels)[i];
		if (data.normal.cross(data.end - data.start) > 0) {
			std::swap(data.end, data.start);
		}
	}
}

//ִֻ��һ��
void FoamVortexSolver::computeBoundaryMatrix() {
	auto panels = _foamVortexData->panelSet;
	auto panleSize = panels->size();
	auto& A = _foamVortexData->A;

	A.resize(panleSize, panleSize);

	for (int j = 0; j < panleSize; ++j) {
		auto normal = panels->lookAt(j).normal;
		for (int i = 0; i < panleSize; ++i) {
			auto mid_i = panels->midPoint(i);
			auto u_ji = computeUnitVelocityFromPanels(j, mid_i).dot(normal);
			A(j, i) = -u_ji;
		}
	}
}

//���������vortex sheet strength
//Ϊ�˼򻯣���panel�ϲ����ٶȵ�������ʱ��һ�����������
void FoamVortexSolver::vortexSheetSolve(double timeIntervalInSeconds) {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto panels = data->panelSet;
	auto panleSize = panels->size();
	auto& pos = data->positions();
	auto& A = _foamVortexData->A;
	auto& x = _foamVortexData->strength;

	//��װb
	Eigen::VectorXd b(panleSize);
	for (int i = 0; i < panleSize; ++i) {
		auto normal = panels->lookAt(i).normal;
		b[i] = vs_vec.dot(normal);
	}
	//cout << b << endl;
	x = A.colPivHouseholderQr().solve(b);
}