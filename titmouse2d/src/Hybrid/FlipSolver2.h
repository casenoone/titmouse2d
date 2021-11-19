#ifndef FLIPSOLVER2_H
#define FLIPSOLVER2_H

#include "PicSolver2.h"

class FlipSolver2 : public PicSolver2 {

public:
	class Builder;

	FlipSolver2();

	FlipSolver2(
		const Vector2<size_t>& resolutions,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	virtual ~FlipSolver2();


	//����pic��ռ�ı���
	double picBlendingFactor()const;

	//���ֵΪ0-1
	//Ϊ�˼���noise�������������
	void setPicBlendingFactor(double factor);

	static Builder builder();

protected:
	void transferFromParticlesToGrids() override;

	void transferFromGridsToParticles() override;

private:
	double _picBlendingFactor = 1;
	Array2Ptr<double> _uDelta;
	Array2Ptr<double> _vDelta;

};

typedef std::shared_ptr<FlipSolver2> FlipSolver2Ptr;


class FlipSolver2::Builder final
	: public GridFluidSolverBuilderBase2<FlipSolver2::Builder> {
public:
	FlipSolver2 build() const;

	FlipSolver2Ptr makeShared() const;
};




#endif