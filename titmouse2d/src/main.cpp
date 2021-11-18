//#include <iostream>
//#include <omp.h>   // NEW ADD
//
//using namespace std;
//
//#include "Geometry/Box2.h"
//#include "Geometry/Plane2.h"
//#include "Collider2.h"
//
//#include <vector>
//
//int main()
//{
//
//    Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(5, 5));
//    Plane2Ptr plane1 = make_shared<Plane2>(Vector2<double>(0, 0), Vector2<double>(8, 7));
//   
//    Vector2<double> P(7, 7);
//
//
//    Collider2 collider;
//    collider.push(box1);
//    collider.push(plane1);
//
//    Collider2::ColliderQueryResult* qResult = new Collider2::ColliderQueryResult;
//
//    collider.getClosestPoint(P, qResult);
//
//    cout << qResult->point.x<<","<<qResult->point.y << endl;
//   cout<<collider.isPenetrating(*qResult, P, 0.0)<<endl ;
//    return 0;
//}
