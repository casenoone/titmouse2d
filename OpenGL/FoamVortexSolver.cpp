#include "FoamVortexSolver.h"

#include "../titmouse2d/src/ConstVar.h"

#include "../titmouse2d/src/random.h"

#include "../titmouse2d/src/Matrix3x3.hpp"

#include "../titmouse2d/src/boundingbox2.h"

#include <omp.h>

#include <iostream>
#include <cmath>
const double eps = 0.00001;

const Vector2D vs_vec = Vector2D(0.1, 0.0);

double fv_eps = 0.01;

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
	auto& positions = _foamVortexData->positions();
	//auto& forces = _foamVortexData->forces();
	auto& vel = _foamVortexData->velocities();

	for (int i = 0; i < n; ++i) {
		Vector2D vortexVel;
		//for (int j = 0; j < n; ++j) {
		//	if (i != j) {
		//		//�����Ҫ��data��������һ����Ա����
		//		//���Ե���Ҫĳ��ʱ�����ӵ��ٶ�ʱ
		//		//���ص�Ӧ����velocity + vortexVel
		//		vortexVel += computeUSingle(positions[i], j);
		//	}
		//}

		Vector2D votexSheetVel;

		//����������߽��ٶ�
		votexSheetVel = computeSingleVelocityFromPanels(i);

		//��ʱ����������
		//��Ҫע����ǣ�����ٶȻ��ֲ�������������������ָ��������ٶȳ�
		_newVelocities[i] = vel[i] + votexSheetVel /* + forces[i] * timeIntervalInSeconds */;

		_newPositions[i] = positions[i] + timeIntervalInSeconds * (_newVelocities[i] + vortexVel);
	}


}

void FoamVortexSolver::onAdvanceTimeStep(double timeIntervalInSeconds) {
	//onBeginAdvanceTimeStep();
	transferFromParticlesToGrids();
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

void FoamVortexSolver::setMovingGrid(const Vector2I& resolution_,
	const BoundingBox2& domain_) {
	_foamVortexData->movingGrid = make_shared<MovingFaceCenteredGrid2>(resolution_, domain_);
}


//�������panel�Ϸ������ӵİ汾
//void FoamVortexSolver::emitParticlesFromPanel() {
//	auto data = _foamVortexData;
//	auto n = data->numberOfParticles();
//	auto panels = data->panelSet;
//	auto& pos = data->positions();
//	//����ÿ��panel����panel�������������
//	auto panelNum = panels->size();
//
//	for (auto i = 0; i < panelNum; ++i) {
//		const auto p = panels->lookAt(i);
//		auto midPoint = 0.5 * (p.start + p.end);
//
//		//�ڱ߽總�������������
//
//		auto line = (p.end - p.start).getNormalize();
//		auto panelLength = (p.end - p.start).getLength();
//
//		int emitNum = 3;
//		for (int j = 0; j < emitNum; ++j) {
//
//			auto lambda = random_double(0.0, panelLength);
//			auto ppos = p.start + lambda * line;
//
//			ppos += random_double(0.03, 0.05) * p.normal;
//
//			pos.push(ppos);
//			data->vorticities().push(0.0);
//			_newVelocities.push(vs_vec);
//			_newPositions.push(Vector2D());
//
//			data->numberOfParticles()++;
//		}
//
//	}
//}

void FoamVortexSolver::emitParticlesFromPanel() {
	auto data = _foamVortexData;
	auto n = data->numberOfParticles();
	auto& pos = data->positions();
	auto& vel = data->velocities();

	double dx = 0.05;
	double dy = 0.05;
	Vector2D center(0.2, 1);
	Vector2D lower(center.x - dx, center.y - dy);
	Vector2D upper(center.x + dx, center.y + dy);

	int emitNum = 5;
	Vector2D tempPos;
	for (int i = 0; i < emitNum; ++i) {

		tempPos.x = random_double(lower.x, upper.x);
		tempPos.y = random_double(lower.y, upper.y);

		pos.push(tempPos);
		vel.push(vs_vec);
		data->vorticities().push(0.0);
		_newVelocities.push(vs_vec);
		_newPositions.push(Vector2D());

		data->numberOfParticles()++;
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
		temp2.y = log10((r2 + fv_eps) / (r1 + fv_eps)) / (2 * kPiD);

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
	result.y = log10((r2 + fv_eps) / (r1 + fv_eps)) / (2 * kPiD);

	result = transToWorld * result;

	return result;
}


//��������Ż�����������ռ�ݵĵط�������P2G����
void FoamVortexSolver::transferFromParticlesToGrids() {
	auto data = _foamVortexData;
	auto& flow = data->movingGrid;

	//��ʱ�������������ָ��������Ǹ��ٶ�
	//��������ϵĻ����˴����ٶȳ�Ӧ��Ҫ�������ָ��������ٶȳ�Ҳ����
	auto& velocities = data->velocities();
	auto& positions = data->positions();
	int n = data->numberOfParticles();

	flow->fill(Vector2D(0.0, 0.0));
	auto sizeU = flow->uSize();
	auto sizeV = flow->vSize();
	auto& u = flow->uDatas();
	auto& v = flow->vDatas();

	Array2D uWeight;
	uWeight.reSize(sizeU.x, sizeU.y, 0.0);
	Array2D vWeight;
	vWeight.reSize(sizeV.x, sizeV.y, 0.0);


	LinearArraySampler2<double> uSampler(
		u,
		flow->gridSpacing(),
		flow->uOrigin()
	);

	LinearArraySampler2<double> vSampler(
		v,
		flow->gridSpacing(),
		flow->vOrigin()
	);

	for (int i = 0; i < n; ++i) {

		std::array<Vector2I, 4> indices;
		std::array<double, 4> weights;

		uSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
		for (int j = 0; j < 4; ++j) {
			u(indices[j].x, indices[j].y) += velocities[i].x * weights[j];
			uWeight(indices[j].x, indices[j].y) += weights[j];
		}

		vSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
		for (int j = 0; j < 4; ++j) {
			v(indices[j].x, indices[j].y) += velocities[i].y * weights[j];
			vWeight(indices[j].x, indices[j].y) += weights[j];
		}


	}

	omp_set_num_threads(23);
#pragma omp parallel for
	for (int i = 0; i < sizeU.x; ++i) {
		for (int j = 0; j < sizeU.y; ++j) {
			if (uWeight(i, j) > 0.0) {
				u(i, j) /= uWeight(i, j);
			}
		}
	}

	omp_set_num_threads(23);
#pragma omp parallel for
	for (int i = 0; i < sizeV.x; ++i) {
		for (int j = 0; j < sizeV.y; ++j) {
			if (vWeight(i, j) > 0.0) {
				v(i, j) /= vWeight(i, j);
			}
		}
	}
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