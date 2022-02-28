#include "IISPHSolver2.h"
#include "../../Lagrangian/SphM4SplineKernel2.h"
#include "../../TempMovingCollider2.h"

const double iisph_omega = 0.5;

const double iisph_rho0 = 0.00000001;


void IISphSolver2::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {
	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);
	initPressure();
}

void IISphSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	ParticleSystemSolver2::beginAdvanceTimeStep();
	ParticleSystemSolver2::initSearchList(_iisphData->positions());
	initDensity();
	computeAdv(timeIntervalInSeconds);
	computeD_ii(timeIntervalInSeconds);
	computeA_ii(timeIntervalInSeconds);
	iterPressureSolver(timeIntervalInSeconds);
	timeIntegration(timeIntervalInSeconds);
	ParticleSystemSolver2::resolveCollision();
	////setMovingColliderPos(resolveMovingCollision(_newPositions, _newVelocities));

	ParticleSystemSolver2::endAdvanceTimeStep();
}


void IISphSolver2::computeD_ii(double timeIntervalInSeconds) {
	auto n = iisphData()->numberOfParticles();
	auto densities = iisphData()->densities();
	auto pos = _iisphData->positions();
	auto neighbors = _iisphData->neighbor->neighBors();
	auto t2 = timeIntervalInSeconds * timeIntervalInSeconds;
	auto d_ii = _iisphData->d_ii;
	d_ii.clear();
	d_ii.reSize(n);

	SphSpikyKernel2 kernel(iisphKR);
	//SphM4SplineKernel2 kernel(iisphKR);

	for (int i = 0; i < n; ++i) {
		auto currentP = pos[i];
		Vector2D temp_d_ii(0.0, 0.0);
		auto rho2 = densities[i] * densities[i];

		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos[*j];
			auto dis = currentP.dis(neighborP);
			auto direction = (neighborP - currentP).normalize();

			if (rho2 > 0.0 && dis > 0.0) {
				temp_d_ii += kernel.gradient(dis, direction) / rho2;
			}
		}
		temp_d_ii *= -t2;
		d_ii[i] = temp_d_ii;
	}
}

//注意pressure的初始化
//初始状态下pressure为0

void IISphSolver2::computeA_ii(double timeIntervalInSeconds) {
	auto n = _iisphData->numberOfParticles();
	auto pressure = _iisphData->pressures();
	auto a_ii = _iisphData->a_ii;
	auto densities = iisphData()->densities();
	auto adv_densities = _iisphData->advRho;
	auto pos = _iisphData->positions();
	auto neighbors = _iisphData->neighbor->neighBors();
	a_ii.clear();
	a_ii.reSize(n);
	adv_densities.clear();
	adv_densities.reSize(n);

	SphSpikyKernel2 kernel(iisphKR);
	//SphM4SplineKernel2 kernel(iisphKR);

	//计算预测密度
	for (int i = 0; i < n; ++i) {
		auto temp = 0.0;
		auto currentP = pos[i];
		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos[*j];
			auto dis = neighborP.dis(currentP);
			auto direction = (neighborP - currentP).normalize();
			auto v_adv_ij = _newVelocities[i] - _newVelocities[*j];
			temp += kernel.gradient(dis, direction).dot(v_adv_ij);
		}
		adv_densities[i] = densities[i] + timeIntervalInSeconds * temp;

		////压强初始化
		pressure[i] *= 0.5;
	}

	auto d_ii = _iisphData->d_ii;
	auto t2 = timeIntervalInSeconds * timeIntervalInSeconds;

	//计算a_ii
	for (int i = 0; i < n; ++i) {
		double rho_adv = 0.0;
		auto currentP = pos[i];
		auto rho_i2 = densities[i] * densities[i];
		auto temp_a_ii = 0.0;
		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos[*j];
			auto dis = neighborP.dis(currentP);
			auto d_ji_direction = (currentP - neighborP).normalize();
			auto direction = -1.0 * d_ji_direction;
			if (dis > 0 && rho_i2 > 0) {
				auto d_ji = -t2 / rho_i2 * kernel.gradient(dis, d_ji_direction);
				temp_a_ii += (d_ii[i] - d_ji).dot(kernel.gradient(dis, direction));
			}

		}
		a_ii[i] = temp_a_ii;
		if (a_ii[i] == 0.0)
			a_ii[i] = 0.1;
	}
}

//先不考虑误差项，通过最大迭代次数来停止迭代
void IISphSolver2::iterPressureSolver(double timeIntervalInSeconds) {
	int maxIterNum = 15;
	int l = 0;
	double t2 = timeIntervalInSeconds * timeIntervalInSeconds;
	auto n = _iisphData->numberOfParticles();

	Array<Vector2D> d_ij_p_j;
	d_ij_p_j.reSize(n);

	SphSpikyKernel2 kernel(iisphKR);
	//SphM4SplineKernel2 kernel(iisphKR);


	auto densities = iisphData()->densities();
	auto pos = _iisphData->positions();
	auto pressure = _iisphData->pressures();
	auto neighbors = _iisphData->neighbor->neighBors();

	while (++l < maxIterNum) {
		//计算d_ij_p_j
		for (int i = 0; i < n; ++i) {
			auto currentP = pos[i];

			auto temp_d_ij = Vector2D(0.0, 0.0);
			for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
				auto rho_j2 = densities[*j] * densities[*j];
				auto neighborP = pos[*j];
				auto dis = neighborP.dis(currentP);
				auto direction = (neighborP - currentP);
				if (dis > 0 && rho_j2 > 0) {
					temp_d_ij += (pressure[*j] * kernel.gradient(dis, direction)) / rho_j2;
				}


			}
			temp_d_ij *= -t2;
			d_ij_p_j[i] = temp_d_ij;
		}


		auto a_ii = _iisphData->a_ii;
		auto d_ii = _iisphData->d_ii;
		auto rho_adv = _iisphData->advRho;
		//计算压强
		for (int i = 0; i < n; ++i) {
			auto term1 = (1 - iisph_omega) * pressure[i];
			if (a_ii[i] == 0.0) {
				//pressure[i] = 0.0;
				//continue;
				//cout << 99 << endl;
			}
			auto term2 = iisph_omega / a_ii[i];
			auto term3 = std::min(0.0, iisph_rho0 - rho_adv[i]);
			double term4 = 0.0;
			auto rho_i2 = densities[i] * densities[i];
			auto currentP = pos[i];
			auto term41 = d_ij_p_j[i];
			for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
				auto neighborP = pos[*j];
				auto dis = neighborP.dis(currentP);
				auto direction = (neighborP - currentP).normalize();

				auto term42 = d_ii[*j] * pressure[*j];
				auto term43 = Vector2D(0.0, 0.0);

				auto d_ji_direction = (currentP - neighborP).normalize();
				if (rho_i2 > 0.0 && dis > 0) {
					auto d_ji = -t2 / rho_i2 * kernel.gradient(dis, d_ji_direction);
					term43 = d_ij_p_j[*j] - d_ji * pressure[i];
				}

				term4 += (term41 + term42 + term43).dot(kernel.gradient(dis, direction));
			}


			pressure[i] = term1 + term2 * (term3 - term4);
			//cout << term3 << endl;
		}
	}
}


void IISphSolver2::timeIntegration(double timeIntervalInSeconds) {
	auto n = _iisphData->numberOfParticles();
	auto pos = _iisphData->positions();
	auto vel = _iisphData->velocities();

	auto densities = iisphData()->densities();
	auto pressure = _iisphData->pressures();
	auto neighbors = _iisphData->neighbor->neighBors();

	SphSpikyKernel2 kernel(iisphKR);
	//SphM4SplineKernel2 kernel(iisphKR);

	for (int i = 0; i < n; ++i) {
		auto currentP = pos[i];
		auto tempPressureForce = Vector2D(0.0, 0.0);
		auto rho_i2 = densities[i] * densities[i];
		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos[*j];
			auto dis = neighborP.dis(currentP);
			auto direction = (neighborP - currentP).normalize();

			auto rho_j2 = densities[*j] * densities[*j];
			if (rho_i2 > 0.0 && rho_j2 > 0.0 && dis > 0.0) {
				tempPressureForce += ((pressure[i] / rho_i2) + (pressure[*j]) / rho_j2) *
					(kernel.gradient(dis, direction));
			}
		}
		tempPressureForce *= -1.0;

		_newVelocities[i] += timeIntervalInSeconds * tempPressureForce;
		_newPositions[i] = pos[i] + timeIntervalInSeconds * _newVelocities[i];
	}

}
