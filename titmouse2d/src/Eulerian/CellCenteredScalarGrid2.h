//#ifndef CELLCENTEREDSCALARGRID2_H
//#define CELLCENTEREDSCALARGRID2_H
//
//#include "ScalarGrid2.h"
//
//class CellCenteredScalarGrid2 final : public ScalarGrid2 {
//public:
//	
//    using  CellCenteredScalarGrid2Ptr = std::shared_ptr<CellCenteredScalarGrid2>;
//	
//    class Builder;
//
//	CellCenteredScalarGrid2();
//	
//    CellCenteredScalarGrid2(
//        const Vector2<size_t>& resolution,
//        const Vector2<double>& origin,
//        double initialValue);
//
//	virtual ~CellCenteredScalarGrid2();
//
//	Vector2<size_t> dataSize() const override;
//
//	Vector2<double> dataOrigin() const override;
//
//	static Builder builder();
//
//};
//
//using  CellCenteredScalarGrid2Ptr = std::shared_ptr<CellCenteredScalarGrid2> ;
//
//
//
//
//
//
//
//
//
//class CellCenteredScalarGrid2::Builder : public ScalarGridBuilder2 {
//public:
//    Builder& withResolution(const Vector2<size_t>& resolution);
//
//    Builder& withResolution(size_t resolutionX, size_t resolutionY);
//
//    Builder& withOrigin(const Vector2<double>& gridOrigin);
//
//    Builder& withOrigin(double gridOriginX, double gridOriginY);
//
//    Builder& withInitialValue(double initialVal);
//
//    CellCenteredScalarGrid2 build() const;
//
//    CellCenteredScalarGrid2Ptr makeShared() const;
//
//    ScalarGrid2Ptr build(
//        const Vector2<size_t>& resolution,
//        const Vector2<double>& gridOrigin,
//        double initialVal) const override;
//
//private:
//	Vector2<size_t> _resolution;
//	Vector2<double> _gridOrigin;
//    double _initialVal;
//};
//
//
//
//
//
//
//#endif