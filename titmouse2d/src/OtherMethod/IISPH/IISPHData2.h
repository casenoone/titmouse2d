#ifndef IISPHDATA2_H
#define IISPHDATA2_H

#include "../../Lagrangian/SphSystemData2.h"

class IISphData2 : public SphSystemData2 {
public:
	IISphData2() = default;

	ArrayD a_ii;

	Array<Vector2D> d_ii;

	ArrayD advRho;

private:

};

using IISphData2Ptr = std::shared_ptr<IISphData2>;

#endif