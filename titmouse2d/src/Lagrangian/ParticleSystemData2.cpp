#include "ParticleSystemData2.h"

ParticleSystemData2::ParticleSystemData2() {

}

ParticleSystemData2::~ParticleSystemData2() {

}

size_t& ParticleSystemData2::numberOfParticles() {
	return _numberOfParticles;
}

Array<Vector2<double>>& ParticleSystemData2::positions() {
	return _positions;
}

Array<Vector2<double>>& ParticleSystemData2::velocities() {
	return _velocities;
}

Array<Vector2<double>>& ParticleSystemData2::forces() {
	return _forces;
}