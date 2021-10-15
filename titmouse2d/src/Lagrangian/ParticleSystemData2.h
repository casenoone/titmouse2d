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
	~ParticleSystemData2();

	size_t &numberOfParticles();

	Array<Vector2<double>>& positions();
	
	Array<Vector2<double>>& velocities();
	
	Array<Vector2<double>>& forces();

	NeighborSearcher2Ptr neighbor;

private:
	
	Array<Vector2<double>> _positions;
	
	Array<Vector2<double>> _velocities;
	
	Array<Vector2<double>> _forces;
	
	size_t _numberOfParticles = 0;

};


#endif