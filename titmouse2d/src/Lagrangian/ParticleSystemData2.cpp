#include "ParticleSystemData2.h"

ParticleSystemData2::ParticleSystemData2() {

}

ParticleSystemData2::~ParticleSystemData2() {

}

int& ParticleSystemData2::numberOfParticles() {
	return _numberOfParticles;
}

Array<Vector2D>& ParticleSystemData2::positions() {
	return _positions;
}

Array<Vector2D>& ParticleSystemData2::velocities() {
	return _velocities;
}

Array<Vector2D>& ParticleSystemData2::forces() {
	return _forces;
}