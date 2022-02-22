#ifndef EXPLICITSURFACE2_H
#define EXPLICITSURFACE2_H

#include <iostream>
using namespace std;

#include <vector>

#include "Surface2.h"
#include "../Vector2.hpp"
#include "../Array.hpp"

class SurfaceElement2 {
public:
	SurfaceElement2();

	virtual ~SurfaceElement2();

	SurfaceElement2(Vector2<double> _start, Vector2<double> _end, Vector2<double> _normal);


public:
	Vector2<double> start;

	Vector2<double> end;

	Vector2<double> normal;

};



//也应该为其写一个Builder类
//后期再写吧
class ExplicitSurface2 : public Surface2 {
public:
	ExplicitSurface2();

	virtual ~ExplicitSurface2();

	ExplicitSurface2(const vector<SurfaceElement2>& data);

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const override;

	virtual double closestDistance(const Vector2<double>& otherPoint)const override;

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint);

	int size()const;

	SurfaceElement2& operator()(int i);

	SurfaceElement2& operator[](int i);

	const SurfaceElement2& lookAt(int i)const;

public:

	vector<SurfaceElement2> _data;


};

using ExplicitSurface2Ptr = shared_ptr<ExplicitSurface2>;


inline SurfaceElement2& ExplicitSurface2::operator()(int i) {
	return _data[i];
}

inline SurfaceElement2& ExplicitSurface2::operator[](int i) {
	return _data[i];
}

inline const SurfaceElement2& ExplicitSurface2::lookAt(int i)const {
	return _data[i];
}


inline int ExplicitSurface2::size()const {
	return _data.size();
}

#endif

