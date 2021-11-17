#ifndef SURFACE2_H
#define SURFACE2_H

#include <iostream>
using namespace std;

//将这个类划分为显式曲面和隐式曲面
//然后再配以显式和隐式之间的转换
class Surface2 {
public:
	Surface2();

	virtual ~Surface2();

	virtual void ghost() const{
		
	}

};

using Surface2Ptr = shared_ptr<Surface2>;


#endif