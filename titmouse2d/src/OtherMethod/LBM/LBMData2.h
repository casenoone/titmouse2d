#ifndef LBMDATA2_H
#define LBMDATA2_H

#include <iostream>
using namespace std;

#include <array>

#include "../../Vector2.hpp"
#include "../../Array2.hpp"



class LBMData2 {
public:

	LBMData2() = default;

	LBMData2(const Vector2<int>& resolution) :_resolution(resolution) {
		f.reSize(resolution.x, resolution.y);
		f_.reSize(resolution.x, resolution.y);
		velocity.reSize(resolution.x, resolution.y);
		rho.reSize(resolution.x, resolution.y);

		for (int i = 0; i < resolution.x; ++i) {
			for (int j = 0; j < resolution.y; ++j) {
				f(i, j).resize(9);
				f_(i, j).resize(9);
			}
		}

	}

	Vector2<int> resolution()const {
		return _resolution;
	}

public:
	Array2<vector<double>> f;
	Array2<vector<double>> f_;

	Array2<Vector2<double>> velocity;

	Array2<double> rho;

	Vector2<int> _resolution;
};







#endif