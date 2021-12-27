#ifndef PARTICLESYSTEMSOLVER2_H
#define PARTICLESYSTEMSOLVER2_H

#include <iostream>
using namespace std;

#include "../ArrayPtr.hpp"
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

	void setData(size_t numberOfParticles, ArrayPtr<Vector2<double>>& pos, size_t resolutionX, size_t resolutionY) ;

	void setCollider(const Collider2& collider);


	virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void accumulateForces(double timeIntervalInSeconds);

public:
	shared_ptr<ParticleSystemData2> _particleSystemData;
	Collider2 _collider;

protected:
	//��ʱ��ʵ��
	//ColliderSet2 _collider;

	virtual void beginAdvanceTimeStep();
	
	virtual void endAdvanceTimeStep();
	
	virtual void timeIntegration(double timeIntervalInSeconds);
	
	virtual void resolveCollision();
	
	virtual void accumlateExternalForces();
	
	void clearForces(ArrayPtr<Vector2<double>>& forces);

	ArrayPtr<Vector2<double>> _newPositions;

	ArrayPtr<Vector2<double>> _newVelocities;

};

#endif