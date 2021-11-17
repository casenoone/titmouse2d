#include <iostream>
#include <omp.h>   // NEW ADD

using namespace std;

#include "Geometry/Box2.h"

#include "Collider2.h"

#include <vector>

int main()
{

    Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0.5), Vector2<double>(2, 2.5));
    Box2 box2(Vector2<double>(0, 0.5), Vector2<double>(3, 3.5));
    Surface2 surface;

    Collider2 collider;
    collider.push(box1);

    return 0;
}
