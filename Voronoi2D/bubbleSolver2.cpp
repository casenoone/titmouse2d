#include "bubbleSolver2.h"

BubbleSolver2::BubbleSolver2() {
	_particleSystemData = std::make_shared<BubbleData2>();
	_bubbleData = std::make_shared<BubbleData2>();
	_bubbleData = std::dynamic_pointer_cast<BubbleData2>(_particleSystemData);
}


void BubbleSolver2::emitParticles() {

}

void BubbleSolver2::timeIntegration(double timeIntervalInSeconds, const Voronoi2& voronoi) {
	//遍历维诺图的边，然后取出点的索引，计算两点之间的受力


}

void BubbleSolver2::onAdvanceTimeStep(double timeIntervalInSeconds, const Voronoi2& voronoi) {

}