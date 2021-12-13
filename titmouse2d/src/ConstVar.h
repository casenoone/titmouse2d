#ifndef CONSTVAR_H
#define CONSTVAR_H

#include <limits>
#include <cmath>
#include "Vector2.hpp"

const double kPiD = 3.14159265358979323846264338327950288;

const double MASS = 1.0;

const Vector2<double> GRAVITY(0.0, -9.8);

const double DRAG_COEFFICIENT = 0.8;

const double R = 0.08;

//epsilonD指浮点数的最小数，可以理解为0
constexpr double kEpsilonD = std::numeric_limits<double>::epsilon();

constexpr double kMaxD = std::numeric_limits<double>::max();

constexpr double kMinD = std::numeric_limits<double>::min();

constexpr size_t kMaxSize = std::numeric_limits<size_t>::max();

constexpr size_t kZeroSize = 0;

//网格法流体专用
const int FLUID = 0;
const int SOLID = -1;
const int AIR = 1;


#endif