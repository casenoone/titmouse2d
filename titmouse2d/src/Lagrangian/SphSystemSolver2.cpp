#include "SphSystemSolver2.h"

#include <iostream>

//基类与父类的类型转换
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

	//初始化密度
	particles->initDensity();

	////初始化压力	
	particles->pressures().reSize(numberOfParticles);
}

void SphSystemSolver2::onBeginAdvanceTimeStep() {
	auto particles = sphSystemData();
	ParticleSystemSolver2::beginAdvanceTimeStep();

	//更新neighborSearchList
	particles->neighbor->setNeiborList(particles->positions());
	//重新计算密度
	particles->updateDensities();
}

void SphSystemSolver2::onEndAdvanceTimeStep() {
	//这个假粘度有何用？
	computePseudoViscosity();
	ParticleSystemSolver2::endAdvanceTimeStep();
}

void SphSystemSolver2::accumulateNonPressureForces(double timeStepInSeconds) {
	//这里有问题
	ParticleSystemSolver2::accumulateForces(timeStepInSeconds);
	//这个函数尚未实现
	//accumulateViscosityForce();
}

void SphSystemSolver2::accumulatePressureForce(double timeStepInSeconds) {

	auto particles = sphSystemData();

	auto x = particles->positions();
	auto d = particles->densities();

	//这里的p是什么？它就是用EOS方程算出来的那个东西
	//记得要初始化p
	auto p = particles->pressures();
	auto f = particles->forces();
	//书上这里写错了好像
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

	//如果模拟的是水，那么targetDensity就是水的密度
	double targetDensity = 1.0;

	//这里的声速到底有没有平方？
	double eosScale = targetDensity * SPEEDOFSOUND * SPEEDOFSOUND;
	//这个值不知道该取多少
	double negativePressureScale = 0.0;

	for (int i = 0; i < numberOfParticles; ++i) {
		//这里的eosExponet()是什么意思？先用EOSEXPONENT代替
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
	//这里的kernel半径又该取多大？暂且设为KERNELRADIUS
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
	//暂时用KERNELRADIUS代替
	SphSpikyKernel2 kernel(KERNEL_RADUIS);
	auto tempNeighbor = particles->neighbor->neighBors();
	for (int i = 0; i < numberOfParticles; ++i) {
		for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
			auto j = *iter;
			double dist = x[i].dis(x[j]);
			//这里不清楚viscosCoefficient()是什么
			double viscosCoefficient = 0.0;
			f[i] = f[i] + (v[j] - v[i]) / d[i]
				* viscosCoefficient * massSquared * kernel.secondDerivative(dist);
		}
	}

}

void SphSystemSolver2::computePseudoViscosity() {

}

//其实这里的绑定执行一次就可以了
SphSystemData2Ptr& SphSystemSolver2::sphSystemData() {
	return _sphSystemData;
}



