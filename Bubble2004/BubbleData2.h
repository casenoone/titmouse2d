#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemData2.h"


class BubbleData2 : public ParticleSystemData2 {
public:
	BubbleData2() = default;

public:

	//����bubble֮���ų�����ϵ��
	double kr = 18;

	//����bubble֮����������ϵ��
	double ka = 15;

	//���ڿ�������
	double kair = 7;

	ArrayD particleRadius;
};

using BubbleDataPtr = std::shared_ptr<BubbleData2>;
