#ifndef PARTICLESYSTEMSOLVER2_H
#define PARTICLESYSTEMSOLVER2_H

#include <iostream>

#include "../Array.hpp"
#include "../Vector2.hpp"

#include "ParticleSystemData2.h"

#include "../Collider2.h"



//��ǰ������������У���ײ������򵥴���

//solver����Ӧ��дһ��Builder�࣬����Ŀǰ�Ȳ�ʵ��

class ParticleSystemSolver2 {
public:

	class Builder;

	ParticleSystemSolver2();
	~ParticleSystemSolver2();

	void setData(int numberOfParticles, Array<Vector2D>& pos, int resolutionX, int resolutionY);

	void setCollider(const Collider2& collider);

	void initSearchList(const Array<Vector2D> pos);

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void accumulateForces(double timeIntervalInSeconds);

public:
	std::shared_ptr<ParticleSystemData2> _particleSystemData;
	Collider2 _collider;

protected:
	//��ʱ��ʵ��
	//ColliderSet2 _collider;

	virtual void beginAdvanceTimeStep();

	virtual void endAdvanceTimeStep();

	virtual void timeIntegration(double timeIntervalInSeconds);

	virtual void resolveCollision();

	virtual void accumlateExternalForces();

	void clearForces(Array<Vector2D>& forces);

	Array<Vector2D> _newPositions;

	Array<Vector2D> _newVelocities;

};


inline void ParticleSystemSolver2::initSearchList(const Array<Vector2D> pos) {
	auto neighbors = _particleSystemData->neighbor;
	neighbors->setNeiborList(pos);
}


#endif