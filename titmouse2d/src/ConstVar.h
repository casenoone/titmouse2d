#ifndef CONSTVAR_H
#define CONSTVAR_H

#include <limits>
#include <cmath>
#include "Vector2.hpp"

const double kPiD = 3.141592653589793238;

const Vector2D GRAVITY(0.0, -9.8);

const double kE = 2.71828182845904523536028747135266249;


//epsilonDָ����������С�����������Ϊ0
constexpr double kEpsilonD = std::numeric_limits<double>::epsilon();

constexpr double kMaxD = std::numeric_limits<double>::max();

constexpr double kMinD = std::numeric_limits<double>::min();

constexpr int kMaxSize = std::numeric_limits<int>::max();

constexpr int kZeroSize = 0;

//��������ר��
const int FLUID = 0;
const int SOLID = -1;
const int AIR = 1;




//sphר��
//ע�������뾶ҪС��gridspacing*2
const double R = 0.08;
const double MASS = 1.0;
const double KERNEL_RADUIS = R;
const double SPEEDOFSOUND = 0.005;
const double EOSEXPONENT = 2.0;
const double DRAG_COEFFICIENT = 0.5;

#endif