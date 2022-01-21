#include "PBFSolver2.h"
#include "../../Lagrangian/SphPoly6Kernel2.h"
#include "../../Lagrangian/SphSpikyKernel2.h"


//这个密度怎么取？
const double rho0 = 100;

const double pbfKR = KERNEL_RADUIS;

void PBFSolver2::setData(int numberOfParticles,
	ArrayPtr<Vector2<double>>& pos,
	int resolutionX,
	int resolutionY) {

}

void PBFSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {

}


void PBFSolver2::onBeginAdvanceTimeStep() {

}

void PBFSolver2::onEndAdvanceTimeStep() {

}


void PBFSolver2::timeIntegration(double timeIntervalInSeconds) {

}


void PBFSolver2::calculateLambda() {

	const double k = 0.5;

	auto n = pbfData()->numberOfParticles();

	for (int i = 0; i < n; ++i) {

	}
}


void PBFSolver2::iterSolve() {

}

void PBFSolver2::initDensity() {

	int n = pbfData()->numberOfParticles();
	auto pos = pbfData()->positions();
	findNeighborParticles(pos);

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