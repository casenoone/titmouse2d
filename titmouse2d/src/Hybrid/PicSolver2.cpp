#include "PicSolver2.h"
#include "../boundingbox2.h"
#include <array>

#include<omp.h>

PicSolver2::PicSolver2() {
}


PicSolver2::~PicSolver2() {
}


PicSolver2::PicSolver2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin)
	: GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	_particles = std::make_shared<ParticleSystemData2>();
}


void PicSolver2::onBeginAdvanceTimeStep(double timeIntervalInSeconds) {
	transferFromParticlesToGrids();
}


void PicSolver2::computePressure(double timeIntervalInSeconds) {
	_pressureSolver->solve(velocity(), cellCenterMarkers);
}


void PicSolver2::onAdvanceTimeStep(double timeIntervalInSeconds) {
	beginAdvanceTimeStep(timeIntervalInSeconds);
	computeExternalForces(timeIntervalInSeconds);
	computePressure(timeIntervalInSeconds);
	computeAdvection(timeIntervalInSeconds);
	endAdvanceTimeStep(timeIntervalInSeconds);
}



void PicSolver2::computeAdvection(double timeIntervalInSeconds) {

	transferFromGridsToParticles();
	moveParticles(timeIntervalInSeconds);
}


//暂时不实现
//void computeViscosity(double timeIntervalInSeconds)override;


void PicSolver2::transferFromParticlesToGrids() {
	auto flow = gridSystemData()->velocity();
	auto positions = _particles->positions();
	auto velocities = _particles->velocities();
	int numberOfParticles = _particles->numberOfParticles();

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

	for (int i = 0; i < numberOfParticles; ++i) {

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

			//这里是为了处理PIC损失重力引起的动能，也可以去掉，没什么影响
			if (velocities[i].y == 0)weights[j] = 0;

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
	setMarkers();
	setFluidCellNum();

}

void PicSolver2::transferFromGridsToParticles() {
	auto flow = gridSystemData()->velocity();
	auto positions = _particles->positions();
	auto velocities = _particles->velocities();
	int numberOfParticles = _particles->numberOfParticles();

	for (int i = 0; i < numberOfParticles; ++i) {
		//注意这个sample怎么写
		velocities[i] = flow->sample(positions[i]);
	}



}


void PicSolver2::moveParticles(double timeIntervalInSeconds) {
	auto flow = gridSystemData()->velocity();
	auto positions = _particles->positions();
	auto velocities = _particles->velocities();
	int numberOfParticles = _particles->numberOfParticles();

	auto lower = flow->origin();
	Vector2D upper;
	upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
	upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;
	//cout << upper.x << " " << upper.y << endl;
	auto  boundingBox = BoundingBox2(lower, upper);
	omp_set_num_threads(23);
#pragma omp parallel for
	for (int i = 0; i < numberOfParticles; ++i) {
		Vector2D pt0 = positions[i];
		Vector2D pt1 = pt0;
		Vector2D vel = velocities[i];

		unsigned int numSubSteps
			= static_cast<unsigned int>(std::max(maxCfl(), 1.0));
		double dt = timeIntervalInSeconds / numSubSteps;
		for (unsigned int t = 0; t < numSubSteps; ++t) {
			Vector2D vel0 = flow->sample(pt0);

			//中点法
			Vector2D midPt = pt0 + vel0 * 0.5 * dt;
			Vector2D midVel = flow->sample(midPt);


			pt1 = pt0 + midVel * dt;
			pt0 = pt1;
		}


		if (pt1.x <= boundingBox.lowerCorner.x) {
			pt1.x = boundingBox.lowerCorner.x;
			vel.x = 0.0;
		}

		if (pt1.x >= boundingBox.upperCorner.x) {
			pt1.x = boundingBox.upperCorner.x;
			vel.x = 0.0;

		}

		if (pt1.y <= boundingBox.lowerCorner.y) {
			pt1.y = boundingBox.lowerCorner.y;
			vel.y = 0.0;
		}
		if (pt1.y >= boundingBox.upperCorner.y) {
			pt1.y = boundingBox.upperCorner.y;
			vel.y = 0.0;
		}


		positions[i] = pt1;
		velocities[i] = vel;
	}


	for (int i = 0; i < numberOfParticles; ++i) {
		collider.resolveCollision(0.00001, 0.01, &positions[i], &velocities[i]);
	}
}



void PicSolver2::setMarkers() {
	auto size = resolution();
	auto positions = particleSystemData()->positions();
	cellCenterMarkers.reSize(size.x, size.y, AIR);
	auto numberOfParticles = particleSystemData()->numberOfParticles();

	omp_set_num_threads(23);
#pragma omp parallel for
	for (int i = 0; i < numberOfParticles; ++i) {
		//作映射
		auto x = positions[i].x;
		auto y = positions[i].y;

		auto xs = (x - gridOrigin().x) / (gridSpacing().x);
		auto ys = (y - gridOrigin().y) / (gridSpacing().y);

		int xss = static_cast<int>(floor(xs));
		int yss = static_cast<int>(floor(ys));
		if (xss >= resolution().x) {
			xss = resolution().x - 1;
		}

		if (yss >= resolution().y) {
			yss = resolution().x - 1;
		}


		cellCenterMarkers(xss, yss) = FLUID;

	}
}

void PicSolver2::setFluidCellNum() {
	int nums = 0;
	auto size = resolution();
	auto vel = velocity();

	vel->solveSystemMarker.reSize(resolution().x, resolution().y, -1);

	//omp_set_num_threads(1);
//#pragma omp parallel for reduction(+:nums)
	//这种情况该怎么并行？
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			//如果格子被流体占据
			if (cellCenterMarkers(i, j) == FLUID) {
				vel->solveSystemMarker(i, j) = nums;
				nums += 1;
			}
		}
	}
}




PicSolver2::Builder PicSolver2::builder() {
	return Builder();
}

void PicSolver2::setCollider(const Collider2& _collider) {
	this->collider = _collider;
}



const ParticleSystemData2Ptr& PicSolver2::particleSystemData() const {
	return _particles;
}

void PicSolver2::setData(int numberOfParticles, Array<Vector2D>& pos, int resolutionX, int resolutionY) {
	_particles->positions() = pos;
	_particles->numberOfParticles() = numberOfParticles;
	_particles->forces().reSize(numberOfParticles);
	_particles->velocities().reSize(numberOfParticles);
}


PicSolver2 PicSolver2::Builder::build() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return PicSolver2(_resolution, gridSpacing, _gridOrigin);
}


PicSolver2Ptr PicSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<PicSolver2>(
		new PicSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](PicSolver2* obj) {
			delete obj;
		});
}