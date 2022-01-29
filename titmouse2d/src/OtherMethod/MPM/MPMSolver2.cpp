#include "MPMSolver2.h"


void MPMSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {

}

void MPMSolver2::setData(int numberOfParticles,
	ArrayPtr<Vector2<double>>& pos,
	int resolutionX,
	int resolutionY) {

}


void MPMSolver2::transferFromParticlesToGrids(double timeIntervalInSeconds) {
	auto flow = _mpmData->g_velocity;
	auto mass = _mpmData->g_mass;
	auto positions = _mpmData->positions();
	auto velocities = _mpmData->velocities();
	auto J = _mpmData->J;
	auto C = _mpmData->C;
	int n = _mpmData->numberOfParticles();
	auto gridSpacing = flow->gridSpacing();

	flow->fill(Vector2D());

	auto size = flow->dataSize();
	auto& grid_v = flow->datas();
	auto grid_m = _mpmData->g_mass;

	Array2Ptr<double> weight;

	LinearArraySampler2<Vector2<double>> sampler(
		grid_v,
		flow->gridSpacing(),
		flow->dataOrigin()
	);

	for (int i = 0; i < n; ++i) {

		auto stress = -timeIntervalInSeconds * 4 * MPM_E * mpm_vol *
			(J(i) - 1) / (gridSpacing.x * gridSpacing.x);

		auto affine = Matrix2x2<double>(stress, 0, 0, stress) + mpm_mass * C(i);

		std::array<Vector2<int>, 4> indices;
		std::array<double, 4> weights;

		sampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
		for (int j = 0; j < 4; ++j) {
			//grid_v(indices[j].x, indices[j].y) += (mpm_mass * velocities[i] + affine*) * weights[j];

		}
	}


}

void MPMSolver2::transferFromGridsToParticles() {

}