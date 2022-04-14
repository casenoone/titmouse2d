#ifndef PARTICLESYSTEMDATA2_H
#define PARTICLESYSTEMDATA2_H

#include <iostream>

#include "../Array.hpp"
#include "../Vector2.hpp"
#include "NeighborSearcher2.h"

class ParticleSystemData2 {
public:
	ParticleSystemData2();
	virtual ~ParticleSystemData2();

	int& numberOfParticles();

	Array<Vector2D>& positions();

	Array<Vector2D>& velocities();

	Array<Vector2D>& forces();

	NeighborSearcher2Ptr neighbor;

private:

	Array<Vector2D> _positions;

	Array<Vector2D> _velocities;

	Array<Vector2D> _forces;

	int _numberOfParticles = 0;

};


using ParticleSystemData2Ptr = std::shared_ptr<ParticleSystemData2>;


#endif