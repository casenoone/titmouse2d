#ifndef SHALLOWWAVEDATA2_H
#define SHALLOWWAVEDATA2_H


class ShallowWaveData2 : public GridSystemData2 {
public:

	ShallowWaveData2() = default;

	ShallowWaveData2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin);

public:
	Array2D height;
};

typedef std::shared_ptr<ShallowWaveData2> ShallowWaveData2Ptr;


#endif