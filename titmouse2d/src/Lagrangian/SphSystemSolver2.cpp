#include "SphSystemSolver2.h"

#include <iostream>

//�����븸�������ת��
SphSystemSolver2::SphSystemSolver2() {
	_particleSystemData = std::make_shared<SphSystemData2>();
	_sphSystemData = std::make_shared<SphSystemData2>();
	_sphSystemData = std::dynamic_pointer_cast<SphSystemData2>(_particleSystemData);

}

SphSystemSolver2::~SphSystemSolver2() {

}

void SphSystemSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {

	onBeginAdvanceTimeStep();
	accumulateForces(timeIntervalInSeconds);

	ParticleSystemSolver2::timeIntegration(timeIntervalInSeconds);
	ParticleSystemSolver2::resolveCollision();
	onEndAdvanceTimeStep();
}

void SphSystemSolver2::accumulateForces(double timeStepInSeconds) {

	accumulateNonPressureForces(timeStepInSeconds);
	accumulatePressureForce(timeStepInSeconds);

}

void SphSystemSolver2::setData(int numberOfParticles, Array<Vector2D>& pos, int resolutionX, int resolutionY) {
	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);

	auto particles = sphSystemData();
	auto neighbor = particles->neighbor->neighBors();

	//��ʼ���ܶ�
	particles->initDensity();

	////��ʼ��ѹ��	
	particles->pressures().reSize(numberOfParticles);
}

void SphSystemSolver2::onBeginAdvanceTimeStep() {
	auto particles = sphSystemData();
	ParticleSystemSolver2::beginAdvanceTimeStep();

	//����neighborSearchList
	particles->neighbor->setNeiborList(particles->positions());
	//���¼����ܶ�
	particles->updateDensities();
}

void SphSystemSolver2::onEndAdvanceTimeStep() {
	//�����ճ���к��ã�
	computePseudoViscosity();
	ParticleSystemSolver2::endAdvanceTimeStep();
}

void SphSystemSolver2::accumulateNonPressureForces(double timeStepInSeconds) {
	//����������
	ParticleSystemSolver2::accumulateForces(timeStepInSeconds);
	//���������δʵ��
	//accumulateViscosityForce();
}

void SphSystemSolver2::accumulatePressureForce(double timeStepInSeconds) {

	auto particles = sphSystemData();

	auto x = particles->positions();
	auto d = particles->densities();

	//�����p��ʲô����������EOS������������Ǹ�����
	//�ǵ�Ҫ��ʼ��p
	auto p = particles->pressures();
	auto f = particles->forces();
	//��������д���˺���
	computePressure();

	accumulatePressureForce(x, d, p, f);


}

double computePressureFromEos(
	double density,
	double targetDensity,
	double eosScale,
	double eosExponent,
	double negativePressureScale) {

	double p = eosScale / eosExponent
		* (pow((density / targetDensity), eosExponent) - 1.0);

	if (p < 0) {
		p *= negativePressureScale;
	}

	return p;

}

void SphSystemSolver2::computePressure() {

	auto particles = sphSystemData();
	auto numberOfParticles = particles->numberOfParticles();
	auto d = particles->densities();
	auto p = particles->pressures();

	//���ģ�����ˮ����ôtargetDensity����ˮ���ܶ�
	double targetDensity = 1.0;

	//��������ٵ�����û��ƽ����
	double eosScale = targetDensity * SPEEDOFSOUND * SPEEDOFSOUND;
	//���ֵ��֪����ȡ����
	double negativePressureScale = 0.0;

	for (int i = 0; i < numberOfParticles; ++i) {
		//�����eosExponet()��ʲô��˼������EOSEXPONENT����
		p[i] = computePressureFromEos(d[i], targetDensity, eosScale, EOSEXPONENT, negativePressureScale);

	}
}



void SphSystemSolver2::accumulatePressureForce(Array<Vector2D> positions,
	ArrayD densities,
	ArrayD pressures,
	Array<Vector2D> pressureForces) {
	auto particles = sphSystemData();
	auto forces = particles->forces();
	auto numberOfParticles = particles->numberOfParticles();
	double massSquared = MASS * MASS;
	//�����kernel�뾶�ָ�ȡ���������ΪKERNELRADIUS
	SphSpikyKernel2 kernel(KERNEL_RADUIS);
	auto tempNeighbor = particles->neighbor->neighBors();
	for (int i = 0; i < numberOfParticles; ++i) {
		for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
			auto j = *iter;
			double dist = positions[i].dis(positions[j]);

			if (dist > 0.0) {
				auto dir = (positions[j] - positions[i]) / dist;
				pressureForces[i] = pressureForces[i] - kernel.gradient(dist, dir) * (pressures[i] / (densities[i] * densities[i])
					+ pressures[j] / (densities[j] * densities[j])) * MASS;

			}
		}
	}
}

void SphSystemSolver2::accumulateViscosityForce() {
	auto particles = sphSystemData();

	auto numberOfParticles = particles->numberOfParticles();
	auto x = particles->positions();
	auto v = particles->velocities();
	auto d = particles->densities();
	auto f = particles->forces();

	double massSquared = MASS * MASS;
	//��ʱ��KERNELRADIUS����
	SphSpikyKernel2 kernel(KERNEL_RADUIS);
	auto tempNeighbor = particles->neighbor->neighBors();
	for (int i = 0; i < numberOfParticles; ++i) {
		for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
			auto j = *iter;
			double dist = x[i].dis(x[j]);
			//���ﲻ���viscosCoefficient()��ʲô
			double viscosCoefficient = 0.0;
			f[i] = f[i] + (v[j] - v[i]) / d[i]
				* viscosCoefficient * massSquared * kernel.secondDerivative(dist);
		}
	}

}

void SphSystemSolver2::computePseudoViscosity() {

}

//��ʵ����İ�ִ��һ�ξͿ�����
SphSystemData2Ptr& SphSystemSolver2::sphSystemData() {
	return _sphSystemData;
}



