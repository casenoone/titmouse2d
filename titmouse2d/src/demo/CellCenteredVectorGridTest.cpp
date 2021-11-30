//#include <iostream>
//using namespace std;
//
//#include "../Eulerian/CellCenteredVectorGrid2.hpp"
//#include "../Eulerian/FaceCenteredGrid2.h"
//#include "../Color3.hpp"
//int main() {
//
//
//	Vector2<size_t> resolution(20, 20);
//	Vector2<double> origin(0.0, 0.0);
//	Color3<double> red(1.0, 0.0, 0.0);
//
//
//
//	auto grid = make_shared<CellCenteredVectorGrid2<Color3<double>>>(resolution,origin, red);
//	
//	Vector2<double> pos(1.5, 1.5);
//    auto result = grid->sample(pos);
//	cout << result.r << endl;
//	return 0;
//}