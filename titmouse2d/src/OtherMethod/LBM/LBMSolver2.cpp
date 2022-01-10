#include "LBMSolver2.h"

#include <array>
#include <tuple>

//明白了，是这样的，格子类型总共有三种：
//1.固体，没有速度之类的属性
//2.有速度的流体
//3.没有速度的流体
LBMSolver2::LBMSolver2(const Vector2<int>& resolution) :_data(resolution) {
	auto res = resolution;
	_g.reSize(res.x, res.y);

	const int border = 3;
	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (i == 0 || i == res.x - 1 || j == 0 || j == res.y - 1) {
				_g(i, j) = LBM_OBS;
			}
			else if (j == res.y - 2 && i > border && i < res.x - (1 + border)) {
				//else if (j == res.y - 2 && i > border && i < border + 3) {
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

void LBMSolver2::setCollider(const Array2Ptr<int>& collider) {

	auto res = resolution();

	for (int i = 0; i < res.x; ++i) {
		for (int j = 0; j < res.y; ++j) {
			if (i != 0 && i != res.x - 1 && j != 0 && j != res.y - 1) {
				if (collider.lookAt(i, j) == LBM_OBS)
					_g(i, j) = LBM_OBS;
			}
		}
	}
}


Vector2<int> LBMSolver2::resolution()const {
	return _data.resolution();
}


void LBMSolver2::stream() {

	auto res = resolution();

	for (int i = 1; i < res.x - 1; ++i) {
		for (int j = 1; j < res.y - 1; ++j) {
			for (int l = 0; l < 8; ++l) {
				auto l_inv = invert[l];

				if (_g(i + e_x[l_inv], j + e_y[l_inv]) == LBM_OBS) {
					_data.f_(i, j)[l] = _data.f(i, j)[l_inv];
				}
				else {
					_data.f_(i, j)[l] = _data.f(i + e_x[l_inv], j + e_y[l_inv])[l];
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
	for (int l = 0; l < 8; ++l) {
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
			auto values = getDensityVelocity(i, j);
			auto rho = _data.rho(i, j) = std::get<0>(values);
			auto u_x = _data.velocity(i, j).x = std::get<1>(values);
			auto u_y = _data.velocity(i, j).y = std::get<2>(values);

			if (_g(i, j) == LBM_VELOCITY)
				u_x = ldc_velocity;

			for (int l = 0; l < 8; ++l) {
				auto a = e_x[l] * u_x + e_y[l] * u_y;

				auto f_eq = w_l[l] * (rho + 1.5 * (u_x * u_x + u_y * u_y) + 3 * a + 4.5 * a * a);
				_data.f(i, j)[l] = (1 - omga) * _data.f_(i, j)[l] + omga * f_eq;
			}

		}
	}
}


Array2Ptr<double> LBMSolver2::getRho()const {
	return _data.rho;
}

const Vector2<double> LBMSolver2::velocityAt(int i, int j)const {

	return _data.velocity.lookAt(i, j);
}



int LBMSolver2::getGridState(int i, int j)const {
	return _g.lookAt(i, j);
}

void LBMSolver2::onAdvancedTimeStep() {
	stream();
	collide();
}