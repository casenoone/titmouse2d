#ifndef ARRAYUTILS_HPP
#define ARRAYUTILS_HPP


#include "Array2.hpp"


template <typename T>
void extrapolateToRegion(
	const Array2<T>& input,
	const Array2I& valid,
	unsigned int numberOfIterations,
	Array2<T> output) {
	auto size = input.dataSize();


	Array2I valid0;
	valid0.reSize(size.x, size.y);
	Array2I valid1;
	valid1.reSize(size.x, size.y);

	//这里可以并行优化
	for (int i = 0; i < size.x; ++i) {
		for (int j = 0; j < size.y; ++j) {
			valid0(i, j) = valid.lookAt(i, j);
			output(i, j) = input.lookAt(i, j);

		}
	}

	for (unsigned int iter = 0; iter < numberOfIterations; ++iter) {

		for (int i = 0; i < size.x; ++i) {
			for (int j = 0; j < size.y; ++j) {
				T sum = T();
				unsigned int count = 0;
				if (!valid0(i, j)) {

					if (i + 1 < size.x && valid0(i + 1, j)) {
						sum = sum + output(i + 1, j);
						++count;
					}

					if (i > 0 && valid0(i - 1, j)) {
						sum = sum + output(i - 1, j);
						++count;
					}

					if (j + 1 < size.y && valid0(i, j + 1)) {
						sum = sum + output(i, j + 1);
						++count;
					}

					if (j > 0 && valid0(i, j - 1)) {
						sum = sum + output(i, j - 1);
						++count;
					}
					if (count > 0) {
						output(i, j)
							= sum / count;
						valid1(i, j) = 1;
					}
				}
				else {
					valid1(i, j) = 1;
				}
			}
		}

		valid0.swap(valid1);
	}
}


#endif