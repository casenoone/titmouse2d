#ifndef PARTICLESYSTEMDATA2_H
#define PARTICLESYSTEMDATA2_H

#include <iostream>
using namespace std;

#include "../Array.hpp"
#include "../Vector2.hpp"
#include "NeighborSearcher2.h"

class ParticleSystemData2 {
public:
	ParticleSystemData2();
	virtual ~ParticleSystemData2();

	int& numberOfParticles();

	Array<Vector2<double>>& positions();

	Array<Vector2<double>>& velocities();

	Array<Vector2<double>>& forces();

	NeighborSearcher2Ptr neighbor;

private:

	Array<Vector2<double>> _positions;

	Array<Vector2<double>> _velocities;

	Array<Vector2<double>> _forces;

	int _numberOfParticles = 0;

};


using ParticleSystemData2Ptr = shared_ptr<ParticleSystemData2>;


#endif