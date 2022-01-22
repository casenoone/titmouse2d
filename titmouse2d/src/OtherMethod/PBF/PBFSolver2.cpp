#include "PBFSolver2.h"
#include "../../Lagrangian/SphPoly6Kernel2.h"
#include "../../Lagrangian/SphSpikyKernel2.h"

const double rho0 = 1.0;

const double pbfKR = KERNEL_RADUIS;

void PBFSolver2::setData(int numberOfParticles,
	ArrayPtr<Vector2<double>>& pos,
	int resolutionX,
	int resolutionY) {
	ParticleSystemSolver2::setData(numberOfParticles, pos, resolutionX, resolutionY);
}

void PBFSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	onBeginAdvanceTimeStep();
	applyForce(timeIntervalInSeconds);
	predictPosition(timeIntervalInSeconds);
	initDensity();
	iterSolve();
	updateVelocites(timeIntervalInSeconds);
	resolveCollision();
	onEndAdvanceTimeStep();
}


void PBFSolver2::onBeginAdvanceTimeStep() {
	ParticleSystemSolver2::beginAdvanceTimeStep();
}

void PBFSolver2::onEndAdvanceTimeStep() {
	ParticleSystemSolver2::endAdvanceTimeStep();
}



void PBFSolver2::calculateLambda() {

	const double epsilon = 0.01;
	auto n = pbfData()->numberOfParticles();
	auto neighbors = pbfData()->neighbor->neighBors();
	auto pos = _newPositions;
	auto density = pbfData()->densities();
	auto lambda = pbfData()->lambda();

	lambda.clear();
	lambda.reSize(n);

	SphSpikyKernel2 kernel(pbfKR);

	//eq(8),eq(10)
	for (int i = 0; i < n; ++i) {
		auto currentP = pos(i);
		Vector2<double> c_grad_temp(0.0, 0.0);
		double c_grad_norm = 0.0;
		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos(*j);
			if (i == *j) {
				for (auto k = neighbors[i].begin(); k != neighbors[i].end(); ++k) {
					auto dis = pos(i).dis(pos(*k));
					if (dis > 0.0) {
						auto direction = (pos(*k) - pos(i)) / dis;
						c_grad_temp += kernel.gradient(dis, direction);
					}

				}
			}
			else {
				auto dis = pos(i).dis(pos(*j));
				auto direction = (pos(*j) - pos(i)) / dis;
				c_grad_temp = kernel.gradient(dis, direction);
			}
		}

		c_grad_norm += ((c_grad_temp / rho0).getLengthSquared());
		auto c_i = (density(i) / rho0) - 1;   //eq(1)
		lambda(i) = -c_i / (c_grad_norm + epsilon);   //eq(11)
	}
}


void PBFSolver2::iterSolve() {

	int iterNum = 20;
	int count = 0;
	while (count < iterNum) {
		calculateLambda();
		calculate_delta_p();
		updatePositions();
		//ParticleSystemSolver2::resolveCollision();
		count++;
	}
}

void PBFSolver2::initDensity() {

	int n = pbfData()->numberOfParticles();
	auto pos = _newPositions;
	findNeighborParticles(pos);

	pbfData()->densities().reSize(n);

	auto neighbor = pbfData()->neighbor->neighBors();
	vector<double> tempData;

	SphPolyKernel2 kernel(pbfKR);

	for (int i = 0; i < n; ++i) {
		double tempDen = 0;
		auto currentP = pos(i);
		for (auto iter = neighbor[i].begin(); iter != neighbor[i].end(); ++iter) {
			auto neighborP = pos(*iter);
			double dis = currentP.dis(neighborP);
			double weight = kernel(dis);
			tempDen += weight;
		}
		if (neighbor[i].size() == 0) {
			tempDen = 0.1;
		}
		tempData.push_back(tempDen);
	}

	pbfData()->densities().set(tempData);

}


void PBFSolver2::calculate_delta_p() {
	const double k = 0.1;
	const int n = 4;
	const double delta_q = 0.2 * pbfKR;

	int numberOfParticles = pbfData()->numberOfParticles();
	auto neighbors = pbfData()->neighbor->neighBors();
	auto pos = _newPositions;
	auto lambda = pbfData()->lambda();
	auto delta_p = pbfData()->deltaP();

	delta_p.clear();
	delta_p.reSize(numberOfParticles);

	SphSpikyKernel2 kernel(pbfKR);

	for (int i = 0; i < numberOfParticles; ++i) {
		auto currentP = pos(i);
		for (auto j = neighbors[i].begin(); j != neighbors[i].end(); ++j) {
			auto neighborP = pos(*j);
			auto dis = neighborP.dis(currentP);
			double S_corr = -k * std::pow((kernel(dis) / kernel(delta_q)), n);
			if (dis > 0.0) {
				auto direction = (neighborP - currentP) / dis;
				delta_p(i) += (lambda(i) + lambda(*j) + S_corr) * kernel.gradient(dis, direction);
			}

		}
		delta_p(i) /= rho0;
	}

}


