#ifndef FDM_UTILS_HPP
#define FDM_UTILS_HPP

#include "../Vector2.hpp"
#include "../Array2.hpp"

#include <iostream>

template<class T>
Vector2<T> gradient2(const Array2<T>& data,
	const Vector2<T>& gridSpacing,
	int i,
	int j);


template<class T>
T laplacian2(
	const Array2<T>& data,
	const Vector2<T>& gridSpacing,
	int i,
	int j);



//这里用的是中心差分法
template<class T>
Vector2<T> gradient2(const Array2<T>& data,
	const Vector2<T>& gridSpacing,
	int i,
	int j) {

	auto tempData = data;
	Vector2<T> ds = tempData.dataSize();

	double left = tempData((i > 0) ? i - 1 : i, j);
	double right = tempData((i + 1 < ds.x()) ? i + 1 : i, j);
	double down = tempData(i, (j > 0) ? j - 1 : j);
	double up = tempData(i, (j + 1 < ds.y()) ? j + 1 : j);

	return Vector2<T>(right - left, up - down) * 0.5 / gridSpacing;

}


template<class T>
T laplacian2(const Array2<T>& data,
	const Vector2<T>& gridSpacing,
	int i,
	int j) {
	auto tempData = data;

	double center = tempData(i, j);
	Vector2<T> ds = tempData.dataSize();

	double dleft = 0.0;
	double dright = 0.0;
	double ddown = 0.0;
	double dup = 0.0;

	if (i > 0) {
		dleft = center - tempData(i - 1, j);
	}
	if (i + 1 < ds.x) {
		dright = tempData(i + 1, j) - center;
	}

	if (j > 0) {
		ddown = center - tempData(i, j - 1);
	}
	if (j + 1 < ds.y) {
		dup = tempData(i, j + 1) - center;
	}

	return (dright - dleft) / (gridSpacing.x * gridSpacing.x)
		+ (dup - ddown) / (gridSpacing.y * gridSpacing.y);
}


#endif