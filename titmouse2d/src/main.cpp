#include <iostream>
#include <omp.h>   // NEW ADD

using namespace std;

#include "Geometry/Box2.h"
#include "Geometry/Plane2.h"
#include "Collider2.h"

#include <vector>

int main()
{

    Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(5, 5));
    Plane2Ptr plane1 = make_shared<Plane2>(Vector2<double>(0, 0), Vector2<double>(5, 5));
    Vector2<double> P(1, 1);

    auto result = box1->getClosedInformation(P);

    //cout << result.point.x << "," << result.point.y << "," << result.distance << endl;

    cout << plane1->normal.x << "," << plane1->normal.y << endl;

    Collider2 collider;
    collider.push(box1);



    return 0;
}
