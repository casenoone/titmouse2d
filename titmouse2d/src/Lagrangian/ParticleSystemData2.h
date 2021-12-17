#ifndef PARTICLESYSTEMDATA2_H
#define PARTICLESYSTEMDATA2_H

#include <iostream>
using namespace std;

#include "../ArrayPtr.hpp"
#include "../Vector2.hpp"
#include "NeighborSearcher2.h"

class ParticleSystemData2 {
public:
	ParticleSystemData2();
	virtual ~ParticleSystemData2();

	size_t &numberOfParticles();

	ArrayPtr<Vector2<double>>& positions();
	
	ArrayPtr<Vector2<double>>& velocities();
	
	ArrayPtr<Vector2<double>>& forces();

	NeighborSearcher2Ptr neighbor;

private:
	
	ArrayPtr<Vector2<double>> _positions;
	
	ArrayPtr<Vector2<double>> _velocities;
	
	ArrayPtr<Vector2<double>> _forces;
	
	size_t _numberOfParticles = 0;

};


using ParticleSystemData2Ptr = shared_ptr<ParticleSystemData2>;


#endif