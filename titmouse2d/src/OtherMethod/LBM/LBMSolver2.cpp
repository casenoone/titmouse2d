#include "LBMSolver2.h"

#include <array>
#include <tuple>

void LBMSolver2::setData() {

}

void LBMSolver2::initLBM() {
	auto res = resolution();
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (i == 0 || i == res.x - 1 || j == 0 || j == res.y - 1) {
				_g(i, j) = OBS;
			}
			else {
				_g(i, j) = FLUID;
			}

			for (int l = 0; l < 9; ++l) {
				_data.f(i, j)[l] = w_l[l];
				_data.f_(i, j)[l] = w_l[l];
			}

		}
	}
}


void LBMSolver2::stream() {

	auto res = resolution();

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			for (int l = 0; l < 8; ++l) {
				auto l_inv = invert[l];
				if (_g(i + e_x[l_inv], j + e_y[l_inv]) == OBS) {
					_data.f_(i, j)[l] = _data.f(i, j)[l_inv];
				}
				else {
					_data.f_(i, j)[l] = _data.f(i + e_x[l_inv], j + e_y[l_inv])[l];
				}
			}
		}
	}
}


void LBMSolver2::collide() {
	auto res = resolution();

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (_g(i, j) == OBS)continue;
			auto values = getDensityVelocity(i, j);
			auto rho = _data.rho(i, j) = std::get<0>(values);
			auto u_x = _data.velocity(i, j).x = std::get<1>(values);
			auto u_y = _data.velocity(i, j).y = std::get<2>(values);

			for (int l = 0; l < 8; ++l) {
				auto a = e_x[l] * u_x + e_y[l] * u_y;

				auto f_eq = w_l[l] * (rho + 1.5 * (u_x * u_x + u_y * u_y) + 3 * a + 4.5 * a * a);
				_data.f(i, j)[l] = (1 - omga) * _data.f_(i, j)[l] + omga * f_eq;
			}

		}
	}
}



auto LBMSolver2::getDensityVelocity(int i, int j) {
	double rho, u_x, u_y;
	rho = u_x = u_y = 0;

	for (int l = 0; l < 8; ++l) {
		rho += _data.f_(i, j)[l];
		u_x += e_x[l] * _data.f_(i, j)[l];

	}

	return std::make_tuple(rho, u_x, u_y);
}


void LBMSolver2::onAdvancedTimeStep() {
	stream();
	collide();
}