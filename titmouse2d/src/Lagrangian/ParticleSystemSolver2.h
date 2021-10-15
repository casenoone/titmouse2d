#ifndef PARTICLESYSTEMSOLVER2_H
#define PARTICLESYSTEMSOLVER2_H

#include <iostream>
using namespace std;

#include "../Array.hpp"
#include "../Vector2.hpp"

#include "ParticleSystemData2.h"
//当前的粒子求解器中，碰撞检测作简单处理

//solver类里应当写一个Builder类，但是目前先不实现

class ParticleSystemSolver2 {
public:
	ParticleSystemSolver2();
	~ParticleSystemSolver2();

	void setData(size_t numberOfParticles, Array<Vector2<double>>& pos, size_t resolutionX, size_t resolutionY) ;

	//碰撞体先不写
	//void setCollider(ColliderSet2& collider_);

	virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

	virtual void accumulateForces(double timeIntervalInSeconds);

public:
	shared_ptr<ParticleSystemData2> _particleSystemData;

protected:
	//暂时不实现
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