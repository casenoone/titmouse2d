#include "MPMSolver2.h"
#include "../../boundingbox2.h"

void MPMSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	transferFromParticlesToGrids(timeIntervalInSeconds);
	transferFromGridsToParticles(timeIntervalInSeconds);
}

void MPMSolver2::setData(int numberOfParticles,
	Array<Vector2D>& pos,
	int resolutionX,
	int resolutionY) {
	_mpmData->positions() = pos;
	_mpmData->numberOfParticles() = numberOfParticles;
	_mpmData->forces().reSize(numberOfParticles);
	_mpmData->velocities().reSize(numberOfParticles);
	initMPMData();

}


//记得写写注释
void MPMSolver2::transferFromParticlesToGrids(double timeIntervalInSeconds) {
	auto flow = _mpmData->g_velocity;
	auto mass = _mpmData->g_mass;
	auto positions = _mpmData->positions();
	auto velocities = _mpmData->velocities();
	auto J = _mpmData->J;
	auto C = _mpmData->C;
	int n = _mpmData->numberOfParticles();
	auto gridSpacing = flow->gridSpacing();
	auto posFunc = flow->dataPosition();

	const auto hh = flow->gridSpacing() / 2.0;
	auto lower = flow->origin();
	Vector2D upper;
	upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
	upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;

	auto  bbox = BoundingBox2(lower, upper);


	flow->fill(Vector2D());
	mass->fill(0.0);
	auto size = flow->dataSize();
	auto& grid_v = flow->datas();
	auto grid_m = mass->datas();

	Array2D weight;
	weight.reSize(flow->resolution().x, flow->resolution().y, 0.0);

	LinearArraySampler2<Vector2D> sampler(
		grid_v,
		flow->gridSpacing(),
		flow->dataOrigin()
	);

	for (int i = 0; i < n; ++i) {
		auto stress = -timeIntervalInSeconds * 4 * MPM_E * mpm_vol *
			(J(i) - 1) / (gridSpacing.x * gridSpacing.x);

		auto posClamped = positions.lookAt(i);
		posClamped.x = clamp(
			posClamped.x, bbox.lowerCorner.x + hh.x,
			bbox.upperCorner.x - hh.x);

		posClamped.y = clamp(
			posClamped.y, bbox.lowerCorner.y + hh.y,
			bbox.upperCorner.y - hh.y);

		auto affine = Matrix2x2<double>(stress, 0, 0, stress) + mpm_mass * C(i);
		std::array<Vector2I, 4> indices;
		std::array<double, 4> weights;


		sampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
		for (int j = 0; j < 4; ++j) {
			auto gridPos = posFunc(indices[j].x, indices[j].y);
			auto dpos = gridPos - posClamped;




			//grid_v(indices[j].x, indices[j].y) += velocities[i] * weights[j];

			//grid_m(indices[j].x, indices[j].y) += weights[j];
			//weight(indices[j].x, indices[j].y) += weights[j];
			//cout << grid_m(indices[j].x, indices[j].y) << "   " << weight(indices[j].x, indices[j].y) << endl;

			grid_v(indices[j].x, indices[j].y) += (mpm_mass * velocities[i] + affine * dpos) * weights[j];
			grid_m(indices[j].x, indices[j].y) += weights[j] * mpm_mass;
		}
	}



	int bound = 2;
	int res_X = flow->resolution().x;
	for (int i = 0; i < grid_m.dataSize().x; ++i) {
		for (int j = 0; j < grid_m.dataSize().y; ++j) {
			if (grid_m(i, j) > 0) {
				grid_v(i, j) /= grid_m(i, j);
			}
			//MPM的边界处理在网格上，这里暂时把重力临时累加到这里
			//注意MPM的边界处理
			grid_v(i, j).y += timeIntervalInSeconds * -15;
			if (i < bound && grid_v(i, j).x < 0.0)
				grid_v(i, j).x = 0.0;
			if (i > res_X - bound && grid_v(i, j).x > 0.0)
				grid_v(i, j).x = 0.0;
			if (j < bound && grid_v(i, j).y < 0.0)
				grid_v(i, j).y = 0.0;
			if (j > res_X - bound && grid_v(i, j).y > 0.0)
				grid_v(i, j).y = 0.0;
		}
	}
}

void MPMSolver2::transferFromGridsToParticles(double timeIntervalInSeconds) {
	auto flow = _mpmData->g_velocity;
	auto mass = _mpmData->g_mass;
	auto positions = _mpmData->positions();
	auto velocities = _mpmData->velocities();
	auto J = _mpmData->J;
	auto C = _mpmData->C;
	int n = _mpmData->numberOfParticles();
	auto gridSpacing = flow->gridSpacing();
	auto posFunc = flow->dataPosition();

	const auto hh = flow->gridSpacing() / 2.0;
	auto lower = flow->origin();
	Vector2D upper;
	upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
	upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;

	auto  bbox = BoundingBox2(lower, upper);


	auto size = flow->dataSize();
	auto& grid_v = flow->datas();
	auto grid_m = _mpmData->g_mass->datas();

	Array2D weight;

	LinearArraySampler2<Vector2D> sampler(
		grid_v,
		flow->gridSpacing(),
		flow->dataOrigin()
	);

	for (int i = 0; i < n; ++i) {


		auto posClamped = positions.lookAt(i);
		posClamped.x = clamp(
			posClamped.x, bbox.lowerCorner.x + hh.x,
			bbox.upperCorner.x - hh.x);

		posClamped.y = clamp(
			posClamped.y, bbox.lowerCorner.y + hh.y,
			bbox.upperCorner.y - hh.y);

		Vector2D new_v;
		Matrix2x2<double> new_C;

		std::array<Vector2I, 4> indices;
		std::array<double, 4> weights;

		auto dx = flow->gridSpacing().x;
		auto dx2 = dx * dx;
		sampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
		for (int j = 0; j < 4; ++j) {
			auto gridPos = posFunc(indices[j].x, indices[j].y);
			auto dpos = gridPos - posClamped;
			auto g_v = grid_v(indices[j].x, indices[j].y);

			//由于数据结构设计上的缺陷，这里暂时这样凑合着实现一下外积
			Matrix2x2<double> mat(g_v.x * dpos.x, g_v.x * dpos.y, g_v.y * dpos.x, g_v.y * dpos.y);
			new_C += 4 * weights[j] * mat / dx2;
		}
		velocities[i] = flow->sample(positions[i]);
		//cout << velocities[0].y << endl;
		positions[i] += timeIntervalInSeconds * velocities[i];
		C[i] = new_C;
		J[i] *= 1 + timeIntervalInSeconds * new_C.trace();
		//cout << J[i] << endl;
	}
}


void MPMSolver2::initMPMData() {
	auto J = _mpmData->J;
	auto C = _mpmData->C;
	auto n = _mpmData->numberOfParticles();

	J.reSize(n);
	C.reSize(n);

	for (int i = 0; i < n; ++i) {
		J[i] = 1.0;
	}
	auto mpm_rho = 1;
	auto dx = _mpmData->g_velocity->gridSpacing();
	mpm_vol = std::pow(0.5 * dx.x, 2.0);
	mpm_mass = mpm_vol * mpm_rho;
}