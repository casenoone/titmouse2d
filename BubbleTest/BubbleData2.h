#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"


class BubbleData2 : public ParticleSystemData2 {
public:
	BubbleData2() = default;

public:
	ArrayD particleRadius;
};

using BubbleDataPtr = std::shared_ptr<BubbleData2>;
