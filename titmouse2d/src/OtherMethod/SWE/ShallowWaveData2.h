#ifndef SHALLOWWAVEDATA2_H
#define SHALLOWWAVEDATA2_H


class ShallowWaveData2 : public GridSystemData2 {
public:

	ShallowWaveData2() = default;

	ShallowWaveData2(
		const Vector2<int>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& origin);
};

typedef std::shared_ptr<ShallowWaveData2> ShallowWaveData2Ptr;


#endif