#ifndef GRID2_HPP
#define GRID2_HPP

#include <iostream>
using namespace std;

#include <functional>

#include "../Vector2.hpp"


class Grid2 {
public:
	
	using Grid2Ptr = shared_ptr<Grid2>;
	
	Grid2();
	virtual ~Grid2();

	//下标转为位置
	using DataPositionFunc = function<Vector2<double>(size_t, size_t)>;

	const Vector2<size_t>& resolution()const;
	
	const Vector2<double>& origin()const;
	
	const Vector2<double>& gridSpacing()const;

	DataPositionFunc cellCenterPosition() const;

	template<typename Callback>
	void forEachCellIndex(Callback& func) const;



protected:
	Vector2<size_t> _resolution;
	Vector2<double> _gridSpacing;
	Vector2<double> _origin;
};


using Grid2Ptr = shared_ptr<Grid2>;




#endif