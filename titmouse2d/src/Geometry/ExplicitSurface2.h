#ifndef EXPLICITSURFACE2_H
#define EXPLICITSURFACE2_H

#include <iostream>

#include <vector>

#include "Surface2.h"
#include "../Vector2.hpp"
#include "../Array.hpp"

class SurfaceElement2 {
public:
	SurfaceElement2();

	virtual ~SurfaceElement2();

	SurfaceElement2(Vector2D _start, Vector2D _end, Vector2D _normal);


public:
	Vector2D start;

	Vector2D end;

	Vector2D normal;


};



//也应该为其写一个Builder类
//后期再写吧
class ExplicitSurface2 : public Surface2 {
public:
	ExplicitSurface2();

	virtual ~ExplicitSurface2();

	ExplicitSurface2(const std::vector<SurfaceElement2>& data);

	virtual Vector2D closestPoint(const Vector2D& otherPoint)const override;

	virtual double closestDistance(const Vector2D& otherPoint)const override;

	virtual SurfaceQueryResult getClosedInformation(const Vector2D& otherPoint);

	int size()const;

	SurfaceElement2& operator()(int i);

	SurfaceElement2& operator[](int i);

	const SurfaceElement2& lookAt(int i)const;

	Vector2D midPoint(int i)const;



public:

	std::vector<SurfaceElement2> _data;

	Vector2D velocity = Vector2D::zero();
};

using ExplicitSurface2Ptr = std::shared_ptr<ExplicitSurface2>;


inline SurfaceElement2& ExplicitSurface2::operator()(int i) {
	return _data[i];
}

inline SurfaceElement2& ExplicitSurface2::operator[](int i) {
	return _data[i];
}

inline const SurfaceElement2& ExplicitSurface2::lookAt(int i)const {
	return _data[i];
}

inline Vector2D ExplicitSurface2::midPoint(int i)const {
	return 0.5 * (_data[i].start + _data[i].end);
}

inline int ExplicitSurface2::size()const {
	return _data.size();
}

#endif

