#ifndef VORONOI_H
#define VORONOI_H

#include "VoronoiData2.h"

class Voronoi2 {
public:
	Voronoi2() = default;

	void generateVoronoi(int number, double width, double height);

	void generateVoronoi(const Array<Vector2D>& positions);


private:
	void addEvent(std::tuple<Vector2D, bool, VoronoiData2::Node>& event, int index);

	void circleCheck(VoronoiData2::Node* node);

	Vector2D getEdgeIntersection(const VoronoiData2::Edge* left, const VoronoiData2::Edge* right);

	double getY(Vector2D& focus, Vector2D& eventpoint);

	VoronoiData2::Node* getNodeOverPoint(const Vector2D& point);

	std::tuple<double, double, double> getEquationOfParabola(Vector2D point, double sweepY);

	void removeEvent(std::tuple<Vector2D, bool, VoronoiData2::Node>* event);

public:
	VoronoiData2 _data;
};


#endif