//#include <iostream>
//using namespace std;
//
//#include "../Eulerian/CellCenteredVectorGrid2.hpp"
//#include "../Eulerian/FaceCenteredGrid2.h"
//#include "../Color3.hpp"
//int main() {
//
//
//	Vector2<size_t> resolution(4, 4);
//	Vector2<double> origin(0.0, 0.0);
//	Color3<double> red(1.0, 0.0, 0.0);
//
//
//
//	auto grid = make_shared<CellCenteredVectorGrid2<Color3<double>>>(resolution,origin, red);
//	
//	(*grid)(0, 0) = Color3<double>(0,0,0.5);
//	(*grid)(0, 1) = Color3<double>(1,0,1);
//	(*grid)(1, 0) = Color3<double>(1,2,1);
//	(*grid)(1, 1) = Color3<double>(0,1,0.5);
//
//	//cout << (*grid)(0, 0).r << endl;
//	auto data = grid->datas();
//
//	Vector2<double> pos(0.5,0.5);
//    auto result = grid->sample(pos);
//	cout << result.r << ", " << result.g << ", " << result.b << endl;
//	return 0;
//}