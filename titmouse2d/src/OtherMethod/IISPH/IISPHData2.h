#ifndef IISPHDATA2_H
#define IISPHDATA2_H

#include "../../Lagrangian/SphSystemData2.h"

class IISphData2 : public SphSystemData2 {
public:
	IISphData2() = default;

	ArrayPtr<double> a_ii;

	ArrayPtr<Vector2<double>> d_ii;

	ArrayPtr<double> advRho;

private:

};

using IISphData2Ptr = shared_ptr<IISphData2>;

#endif