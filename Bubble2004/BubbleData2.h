#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"


class BubbleData2 : public ParticleSystemData2 {
public:
	BubbleData2() = default;

public:

	//调节bubble之间排斥力的系数
	double kr = 18;

	//调节bubble之间吸引力的系数
	double ka = 15;

	//调节空气阻力
	double kair = 7;

	ArrayD particleRadius;
};

using BubbleDataPtr = std::shared_ptr<BubbleData2>;
