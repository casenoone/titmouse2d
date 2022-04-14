#ifndef GRID2_HPP
#define GRID2_HPP

#include <iostream>

#include <functional>

#include "../Vector2.hpp"


class Grid2 {
public:

	using Grid2Ptr = std::shared_ptr<Grid2>;

	Grid2();
	virtual ~Grid2();

	//下标转为位置
	using DataPositionFunc = std::function<Vector2D(int, int)>;

	const Vector2I& resolution()const;

	const Vector2D& origin()const;

	const Vector2D& gridSpacing()const;

	DataPositionFunc cellCenterPosition() const;

	template<typename Callback>
	void forEachCellIndex(Callback& func) const;



protected:
	Vector2I _resolution;
	Vector2D _gridSpacing;
	Vector2D _origin;
};


using Grid2Ptr = std::shared_ptr<Grid2>;




#endif