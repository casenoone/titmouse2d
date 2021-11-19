#ifndef GRIDFLUIDSOLVER2_H
#define GRIDFLUIDSOLVER2_H

#include "../Vector2.hpp"
#include "GridSystemData2.h"

class GridFluidSolver2 {
public:
	class Builder;

	GridFluidSolver2();

	~GridFluidSolver2();

	GridFluidSolver2(const Vector2<size_t>& resolution, const Vector2<double>& gridSpacing, const Vector2<double>& gridOrigin);

    Vector2<size_t> resolution() const;

    Vector2<double> gridSpacing() const;

    Vector2<double> gridOrigin() const;

    virtual void onAdvanceTimeStep(double timeIntervalInSeconds);

    static Builder builder();

protected:

    virtual void onBeginAdvanceTimeStep(double timeIntervalInSeconds);

    virtual void onEndAdvanceTimeStep(double timeIntervalInSeconds);

    virtual void computeExternalForces(double timeIntervalInSeconds);

    virtual void accumulateForces(double timeIntervalInSeconds);

    virtual void computePressure(double timeIntervalInSeconds);

    virtual void computeAdvection(double timeIntervalInSeconds);

    void computeGravity(double timeIntervalInSeconds);

    void beginAdvanceTimeStep(double timeIntervalInSeconds);

    void endAdvanceTimeStep(double timeIntervalInSeconds);

    void updateCollider(double timeIntervalInSeconds);
    
    const Vector2<double>& gravity() const;

    //暂时不实现
    //double cfl(double timeIntervalInSeconds) const;

    double maxCfl() const;

    //暂时不实现
    //void setMaxCfl(double newCfl);

    const GridSystemData2Ptr& gridSystemData() const;

    

    FaceCenteredGrid2Ptr velocity();

private:
    Vector2<double> _gravity = Vector2<double>(0.0, -9.8);
    double _viscosityCoefficient = 0.0;
    double _maxCfl = 5.0;

    GridSystemData2Ptr _grids;
    //Collider2Ptr _collider;
};


typedef std::shared_ptr<GridFluidSolver2> GridFluidSolver2Ptr;



template <typename DerivedBuilder>
class GridFluidSolverBuilderBase2 {
public:
    DerivedBuilder& withResolution(const Vector2<size_t>& resolution);

    DerivedBuilder& withOrigin(const Vector2<double>& gridOrigin);

protected:
    Vector2<size_t> _resolution;
    Vector2<double> _gridSpacing;
    Vector2<double> _gridOrigin;

};



template <typename T>
T& GridFluidSolverBuilderBase2<T>::withResolution(const Vector2<size_t>& resolution) {
    _resolution = resolution;
    return static_cast<T&>(*this);
}


template <typename T>
T& GridFluidSolverBuilderBase2<T>::withOrigin(const Vector2<double>& gridOrigin) {
    _gridOrigin = gridOrigin;
    return static_cast<T&>(*this);
}


class GridFluidSolver2::Builder final
    : public GridFluidSolverBuilderBase2<GridFluidSolver2::Builder> {
public:
    GridFluidSolver2 build() const;

    GridFluidSolver2Ptr makeShared() const {
        
        auto gridSpacing = Vector2<double>(2.0 / _resolution.x, 2.0 / _resolution.y);
        return std::make_shared<GridFluidSolver2>(_resolution, gridSpacing, _gridOrigin);
    }
};

#endif