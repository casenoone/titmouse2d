#include "PicSolver2.h"
#include "../boundingbox2.h"
#include <array>

PicSolver2::PicSolver2() {
}


PicSolver2::~PicSolver2() {
}


PicSolver2::PicSolver2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin)
	: GridFluidSolver2(resolution, gridSpacing, gridOrigin) {
	auto grids = gridSystemData();

	_particles = std::make_shared<ParticleSystemData2>();
}


void PicSolver2::onBeginAdvanceTimeStep(double timeIntervalInSeconds) {
	transferFromParticlesToGrids();
}


//暂时不实现
void PicSolver2::computePressure(double timeIntervalInSeconds) {
	//_pressureSolver->solver(velocity(), cellCenterMarkers);
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
    size_t numberOfParticles = _particles->numberOfParticles();

    flow->fill(Vector2<double>());
    auto sizeU = flow->uSize();
    auto sizeV = flow->vSize();

    auto& u = flow->uDatas();
    auto& v = flow->vDatas();

    Array2Ptr<double> uWeight;
    uWeight.reSize(sizeU.x, sizeU.y, 0.0);
    Array2Ptr<double> vWeight;
    vWeight.reSize(sizeV.x, sizeV.y, 0.0);

    _uMarkers.reSize(u.dataSize().x, u.dataSize().y, 0.0);
    _vMarkers.reSize(v.dataSize().x, v.dataSize().y, 0.0);

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


    for (size_t i = 0; i < numberOfParticles; ++i) {

        std::array<Vector2<int>,4> indices;

        std::array<double, 4> weights;

        uSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 4; ++j) {
            u(indices[j].x, indices[j].y) += velocities[i].x * weights[j];
            uWeight(indices[j].x, indices[j].y) += weights[j];
            _uMarkers(indices[j].x, indices[j].y) = 1;
        }

        vSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 4; ++j) {
            v(indices[j].x, indices[j].y) += velocities[i].y * weights[j];
            vWeight(indices[j].x, indices[j].y) += weights[j];
            _vMarkers(indices[j].x, indices[j].y) = 1;
        }

    }

    for (size_t i = 0; i < sizeU.x; ++i) {
        for (size_t j = 0; j < sizeU.y; ++j) {
            if (uWeight(i, j) > 0.0) {
                u(i, j) /= uWeight(i, j);
            }
        }
    }

    for (size_t i = 0; i < sizeV.x; ++i) {
        for (size_t j = 0; j < sizeV.y; ++j) {
            if (vWeight(i, j) > 0.0) {
                v(i, j) /= vWeight(i, j);

            }
        }
    }


}


void PicSolver2::transferFromGridsToParticles() {
    auto flow = gridSystemData()->velocity();
    auto positions = _particles->positions();
    auto velocities = _particles->velocities();
    size_t numberOfParticles = _particles->numberOfParticles();

    for (size_t i = 0; i < numberOfParticles; ++i) {
        //注意这个sample怎么写
        velocities[i] = flow->sample(positions[i]);

    }

}



void PicSolver2::moveParticles(double timeIntervalInSeconds) {
    auto flow = gridSystemData()->velocity();
    auto positions = _particles->positions();
    auto velocities = _particles->velocities();
    size_t numberOfParticles = _particles->numberOfParticles();

    //BoundingBox2 boundingBox = flow->boundingBox();

    auto lower = flow->origin();
    Vector2<double> upper;
    upper.x = lower.x + flow->resolution().x * flow->gridSpacing().x;
    upper.y = lower.y + flow->resolution().y * flow->gridSpacing().y;

    auto  boundingBox = BoundingBox2(lower, upper);

    for (size_t i = 0; i < numberOfParticles; ++i) {
        Vector2<double> pt0 = positions[i];
        Vector2<double> pt1 = pt0;
        Vector2<double> vel = velocities[i];

  
        unsigned int numSubSteps
            = static_cast<unsigned int>(std::max(maxCfl(), 1.0));

        double dt = timeIntervalInSeconds / numSubSteps;
        for (unsigned int t = 0; t < numSubSteps; ++t) {
            Vector2<double> vel0 = flow->sample(pt0);

            //中点法
            Vector2<double> midPt = pt0 + vel0 * 0.5 * dt;
            Vector2<double> midVel = flow->sample(midPt);
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
        //cout << (*positions)[i].y() << endl;
        velocities[i] = vel;

    }


    //Collider2Ptr col = collider();
    //if (col != nullptr) {
    //    for (size_t i = 0; i < numberOfParticles; ++i) {
    //        col->resolveCollision(0.0, 0.0, &positions[i], &velocities[i]);
    //    }
    //}
}





PicSolver2::Builder PicSolver2::builder() {
	return Builder();
}


const ParticleSystemData2Ptr& PicSolver2::particleSystemData() const {
	return _particles;
}


PicSolver2 PicSolver2::Builder::build() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return PicSolver2(_resolution, gridSpacing, _gridOrigin);
}


PicSolver2Ptr PicSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<PicSolver2>(
		new PicSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](PicSolver2* obj) {
			delete obj;
		});
}