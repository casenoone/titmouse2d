#ifndef PDE_HPP
#define PDE_HPP

#include "../MathUtils.hpp"

//����ֵƫ΢�ִ���������Doyub kim��Fluid Engine Development

template <typename T>
std::array<T, 2> upwind1(T* D0, T dx) {
    T invdx = 1 / dx;
    std::array<T, 2> dfx;
    dfx[0] = invdx * (D0[1] - D0[0]);
    dfx[1] = invdx * (D0[2] - D0[1]);
    return dfx;
}


#endif