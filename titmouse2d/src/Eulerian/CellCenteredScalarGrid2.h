#ifndef CELLCENTEREDSCALARGRID2_H
#define CELLCENTEREDSCALARGRID2_H

#include "ScalarGrid2.h"

class CellCenteredScalarGrid2 final : public ScalarGrid2 {
public:

	using  CellCenteredScalarGrid2Ptr = std::shared_ptr<CellCenteredScalarGrid2>;

	class Builder;

	CellCenteredScalarGrid2();

	CellCenteredScalarGrid2(
		const Vector2I& resolution,
		const Vector2D& origin = Vector2D(),
		double initialValue = 0.0);

	virtual ~CellCenteredScalarGrid2();

	Vector2I dataSize() const override;

	Vector2D dataOrigin() const override;

	static Builder builder();

};

using  CellCenteredScalarGrid2Ptr = std::shared_ptr<CellCenteredScalarGrid2>;


class CellCenteredScalarGrid2::Builder : public ScalarGridBuilder2 {
public:
	Builder& withResolution(const Vector2I& resolution);

	Builder& withResolution(int resolutionX, int resolutionY);

	Builder& withOrigin(const Vector2D& gridOrigin);

	Builder& withOrigin(double gridOriginX, double gridOriginY);

	Builder& withInitialValue(double initialVal);

	CellCenteredScalarGrid2 build() const;

	CellCenteredScalarGrid2Ptr makeShared() const;

	ScalarGrid2Ptr build(
		const Vector2I& resolution,
		const Vector2D& gridOrigin,
		double initialVal) const override;

private:
	Vector2I _resolution;
	Vector2D _gridOrigin;
	double _initialVal;
};






#endif