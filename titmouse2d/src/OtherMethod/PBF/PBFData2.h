#ifndef PBFDATA2_H
#define PBFDATA2_H

#include "../../Lagrangian/ParticleSystemData2.h"

class PBFData2 : public ParticleSystemData2 {
public:
	PBFData2() = default;

public:
	ArrayPtr<double> densities();

	ArrayPtr<double> lambda();

private:
	ArrayPtr<double> _densities;
	ArrayPtr<double> _lambda;
};

using PBFData2Ptr = shared_ptr<PBFData2>;


inline ArrayPtr<double> PBFData2::densities() {
	return _densities;
}

inline ArrayPtr<double> PBFData2::lambda() {
	return _lambda;
}

#endif