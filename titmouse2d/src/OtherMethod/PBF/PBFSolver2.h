#ifndef PBFSOLVER2_H
#define PBFSOLVER2_H


#include "../../Lagrangian/ParticleSystemSolver2.h"
#include "PBFData2.h"

class PBFSolver2 : public ParticleSystemSolver2 {
public:
	PBFSolver2();

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	void setData(int numberOfParticles,
		ArrayPtr<Vector2<double>>& pos,
		int resolutionX,
		int resolutionY);

public:
	PBFData2Ptr& pbfData();

private:
	void onBeginAdvanceTimeStep();

	void onEndAdvanceTimeStep();

	void predictPosition(double timeIntervalInSeconds);

	void calculateLambda();

	void applyForce(double timeIntervalInSeconds);

	void iterSolve();

	void findNeighborParticles(const ArrayPtr<Vector2<double>>& pos);

	void initDensity();

	void calculate_delta_p();

	void updatePositions();

	void updateVelocites(double timeIntervalInSeconds);

	void setMovingColliderPos(double pos);

public:
	double movingColliderPos = 0.0;

private:
	PBFData2Ptr _pbfData;


};


inline void PBFSolver2::setMovingColliderPos(double pos) {
	movingColliderPos = pos;
}

inline PBFSolver2::PBFSolver2() {
	_particleSystemData = make_shared<PBFData2>();
	_pbfData = make_shared<PBFData2>();
	_pbfData = std::dynamic_pointer_cast<PBFData2>(_particleSystemData);
}

inline PBFData2Ptr& PBFSolver2::pbfData() {
	return _pbfData;
}


inline void PBFSolver2::predictPosition(double timeIntervalInSeconds) {
	auto n = pbfData()->numberOfParticles();
	auto pos = pbfData()->positions();

	for (int i = 0; i < n; ++i) {
		_newPositions[i] = pos[i] + timeIntervalInSeconds * _newVelocities[i];
	}
}

inline void PBFSolver2::applyForce(double timeIntervalInSeconds) {
	auto n = pbfData()->numberOfParticles();

	pbfData()->forces().clear();
	pbfData()->forces().reSize(n);
	ParticleSystemSolver2::accumlateExternalForces();

	auto forces = pbfData()->forces();
	auto velocity = pbfData()->velocities();

	for (int i = 0; i < n; ++i) {
		_newVelocities[i] = velocity[i] + timeIntervalInSeconds * forces[i];
	}
}


inline void PBFSolver2::findNeighborParticles(const ArrayPtr<Vector2<double>>& pos) {
	auto particles = pbfData();
	particles->neighbor->setNeiborList(pos);
}


inline void PBFSolver2::updatePositions() {
	auto n = pbfData()->numberOfParticles();
	auto delta_p = pbfData()->deltaP();

	for (int i = 0; i < n; ++i) {
		//if (i == 0)
			//cout << "1:" << _newPositions[0].y << endl;
		_newPositions[i] += delta_p(i);
		//if (i == 0)
			//cout << "2:" << delta_p(0).y << endl;
			//if (i == 0)
				//cout << "3:" << _newPositions[0].y << endl;

	}
}

inline void PBFSolver2::updateVelocites(double timeIntervalInSeconds) {
	auto n = pbfData()->numberOfParticles();
	auto pos = pbfData()->positions();

	for (int i = 0; i < n; ++i) {
		_newVelocities(i) = (_newPositions[i] - pos[i]) / timeIntervalInSeconds;
	}

}

#endif