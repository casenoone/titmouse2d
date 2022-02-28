#include "LBMSolver2.h"
#include "LBM_TurbulenceModel.h"

#include <array>
#include <tuple>

//错误的D2Q9
//const double w_l[9] = { 1.0 / 3, 1.0 / 18, 1.0 / 18,
//						1.0 / 18, 1.0 / 18,
//						1.0 / 18, 1.0 / 18,
//						1.0 / 18, 1.0 / 18 };

const double w_l[9] = { 4.0 / 9, 1.0 / 9, 1.0 / 9,
						1.0 / 9, 1.0 / 9, 1.0 / 36,
						1.0 / 36, 1.0 / 36, 1.0 / 36 };

const int e_x[9] = { 0,1,-1,0,0,1,-1,1,-1 };
const int e_y[9] = { 0,0,0,1,-1,1,1,-1,-1 };

const int invert[9] = { 0, 2, 1, 4, 3, 8, 7, 6, 5 };

//非湍流模型下不能超过1.9
const double omga = 1.9;

//非湍流模型下不能超过0.1
const double ldc_velocity = 0.09;
const bool SMAGORINSKY = false;

LBMSolver2::LBMSolver2(const Vector2I& resolution) :_data(resolution) {

	if (SMAGORINSKY) initSmagoArrays();

	auto res = resolution;
	_g.reSize(res.x, res.y);

	int midNum = res.x / 2;
	const int border = 7;
	int upBorder = midNum + border;
	int downBorder = midNum - border;

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (i == 0 || i == res.x - 1 || j == 0 || j == res.y - 1) {
				_g(i, j) = LBM_OBS;
			}
			//else if (j == res.y - 10 && i > border && i < res.x - (1 + border)) {
			else if (i >= 5 && i <= res.x - 3 && j == res.y - 2) {
				//else if (j >= 1 && j <= res.y - 1 && i == 0) {
				_g(i, j) = LBM_VELOCITY;
			}

			else {
				_g(i, j) = LBM_FLUID;
			}

			for (int l = 0; l < 9; ++l) {
				_data.f(i, j)[l] = w_l[l];
				_data.f_(i, j)[l] = w_l[l];
			}

		}
	}

}

void LBMSolver2::setCollider(const Array2I& collider) {

	auto res = resolution();

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (collider.lookAt(i, j) == LBM_OBS)
				_g(i, j) = LBM_OBS;

		}
	}


}


Vector2I LBMSolver2::resolution()const {
	return _data.resolution();
}


void LBMSolver2::stream() {

	auto res = resolution();

	for (int i = 1; i < res.x - 1; ++i) {
		for (int j = 1; j < res.y - 1; ++j) {
			for (int l = 0; l < 9; ++l) {
				auto l_inv = invert[l];
				if (_g(i, j) != LBM_OBS) {
					auto new_i = i + e_x[l_inv];
					auto new_j = j + e_y[l_inv];
					if (_g(new_i, new_j) == LBM_OBS) {
						_data.f_(i, j)[l] = _data.f(i, j)[l_inv];
					}
					else {
						_data.f_(i, j)[l] = _data.f(i + e_x[l_inv], j + e_y[l_inv])[l];
					}
				}
			}
		}
	}
}

//注意这里用了自动推导类型的形式
//这个函数必须在被调用之前就定义
auto LBMSolver2::getDensityVelocity(int i, int j) {
	double rho, u_x, u_y;
	rho = u_x = u_y = 0.0;
	for (int l = 0; l < 9; ++l) {
		rho += _data.f_(i, j)[l];
		u_x += e_x[l] * _data.f_(i, j)[l];
		u_y += e_y[l] * _data.f_(i, j)[l];
	}

	return std::make_tuple(rho, u_x, u_y);
}



void LBMSolver2::collide() {
	auto res = resolution();
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (_g(i, j) == LBM_OBS) {
				continue;
			}


			if (SMAGORINSKY) {
				collideSmagorinsky(_data.f_(i, j), _g(i, j));

				_data.f(i, j) = _data.f_(i, j);

				double rho = 0, u_x = 0, u_y = 0;
				for (int l = 0; l < 9; ++l) {
					rho += _data.f(i, j)[l];
					u_x += e_x[l] * _data.f_(i, j)[l];
					u_y += e_y[l] * _data.f_(i, j)[l];
				}

				_data.rho(i, j) = rho;

				_data.velocity(i, j).x = u_x / rho;
				_data.velocity(i, j).y = u_y / rho;

				continue;
			}

			auto values = getDensityVelocity(i, j);
			auto rho = std::get<0>(values);
			auto u_x = std::get<1>(values) / rho;
			auto u_y = std::get<2>(values) / rho;

			if (_g(i, j) == LBM_VELOCITY) {
				u_x = ldc_velocity;
			}




			for (int l = 0; l < 9; ++l) {
				auto a = e_x[l] * u_x + e_y[l] * u_y;

				//错误的f_eq计算公式
				//auto f_eq = w_l[l] * (rho - 1.5 * (u_x * u_x + u_y * u_y) + 3 * a + 4.5 * a * a);
				auto f_eq = w_l[l] * rho * (1 + 3 * a + 4.5 * a * a - 1.5 * (u_x * u_x + u_y * u_y));
				_data.f(i, j)[l] = (1 - omga) * _data.f_(i, j)[l] + omga * f_eq;
			}

			_data.rho(i, j) = rho;
			_data.velocity(i, j).x = u_x / rho;
			_data.velocity(i, j).y = u_y / rho;


		}
	}


}


Array2D LBMSolver2::getRho()const {
	return _data.rho;
}

const Vector2D LBMSolver2::velocityAt(int i, int j)const {

	return _data.velocity.lookAt(i, j);
}



int LBMSolver2::getGridState(int i, int j)const {
	return _g.lookAt(i, j);
}

void LBMSolver2::onAdvancedTimeStep() {
	stream();
	collide();
}