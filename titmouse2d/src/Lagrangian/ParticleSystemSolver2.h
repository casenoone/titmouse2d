#ifndef PARTICLESYSTEMSOLVER2_H
#define PARTICLESYSTEMSOLVER2_H

#include <iostream>
using namespace std;

#include "../Array.hpp"
#include "../Vector2.hpp"

#include "ParticleSystemData2.h"
//��ǰ������������У���ײ������򵥴���

//solver����Ӧ��дһ��Builder�࣬����Ŀǰ�Ȳ�ʵ��

class ParticleSystemSolver2 {
public:
	ParticleSystemSolver2();
	~ParticleSystemSolver2();

	void setData(size_t numberOfParticles, Array<Vector2<double>>& pos, size_t resolutionX, size_t resolutionY) ;

	//��ײ���Ȳ�д
	//void setCollider(ColliderSet2& collider_);

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void accumulateForces(double timeIntervalInSeconds);

public:
	shared_ptr<ParticleSystemData2> _particleSystemData;

protected:
	//��ʱ��ʵ��
	//ColliderSet2 _collider;

	virtual void beginAdvanceTimeStep();
	
	virtual void endAdvanceTimeStep();
	
	virtual void timeIntegration(double timeIntervalInSeconds);
	
	virtual void resolveCollision();
	
	virtual void accumlateExternalForces();
	
	void clearForces(Array<Vector2<double>>& forces);

	Array<Vector2<double>> _newPositions;

	Array<Vector2<double>> _newVelocities;

};

#endif