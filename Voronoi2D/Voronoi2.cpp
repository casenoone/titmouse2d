﻿#include "Voronoi2.h"
#include "../titmouse2d/src/random.h"

static const Vector2D error_interSection(-1000, 0);

static double sweepY = 0;

void Voronoi2::generateVoronoi(int number, double width, double height) {
	auto& queue = _data.queue;
	for (auto i = 0; i < number; ++i) {
		auto tempX = random_double(0, 2);
		auto tempY = random_double(0, 2);
		Vector2D point(tempX, tempY);

		VoronoiData2::Node node;
		auto e = new tuple(point, true, node);
		queue.insert(e);
	}

	variant<Vector2D, VoronoiData2::Edge> max_Point = std::get<0>(*queue.maxQ());
	_data.beachline.set(max_Point, 0);

	while (_data.queue.notEmpty()) {
		auto event = queue.maxQ();
		Vector2D ePoint = get<0>(*event);
		sweepY = ePoint.y;
		bool isSiteEvent = get<1>(*event);
		if (isSiteEvent) { addEvent(*event); }
		else {
			removeEvent(event);
		}
	}


}

void Voronoi2::addEvent(tuple<Vector2D, bool, VoronoiData2::Node>& event) {
	auto ePoint = std::get<0>(event);
	_data.sites.push(ePoint);

	variant<Vector2D, VoronoiData2::Edge> eepoint = ePoint;
	VoronoiData2::Node* newNode = new VoronoiData2::Node(eepoint, 0);
	auto node = getNodeOverPoint(ePoint);
	Vector2D start(ePoint.x, getY(node->point, ePoint));

	auto rightEdge = new VoronoiData2::Edge(start, ePoint, node->point);
	auto leftEdge = new VoronoiData2::Edge(start, node->point, ePoint);

	leftEdge->addAdjacent(rightEdge);
	rightEdge->addAdjacent(leftEdge);

	variant<Vector2D, VoronoiData2::Edge> left = *leftEdge;
	variant<Vector2D, VoronoiData2::Edge> right = *rightEdge;
	auto leftNode = new VoronoiData2::Node(left, 1);
	auto rightNode = new VoronoiData2::Node(right, 1);

	variant<Vector2D, VoronoiData2::Edge> nnpoint = node->point;
	auto duplicateNode = new VoronoiData2::Node(nnpoint, 0);

	duplicateNode->setNext(node->next);
	rightNode->setNext(duplicateNode);
	newNode->setNext(rightNode);
	leftNode->setNext(newNode);
	node->setNext(leftNode);


	if (node) {
		_data.queue.removeEvent(node->circle);
		node->circle = nullptr;
	}

	circleCheck(node);
	circleCheck(duplicateNode);
}

void Voronoi2::circleCheck(VoronoiData2::Node* node) {
	VoronoiData2::Edge* leftEdge = nullptr;
	VoronoiData2::Edge* rightEdge = nullptr;
	if (node->prev)
		leftEdge = &node->prev->edge;
	if (node->next)
		rightEdge = &node->next->edge;

	auto intersection = getEdgeIntersection(leftEdge, rightEdge);
	if (intersection.x == -error_interSection.x) {
		return;
	}

	auto dy = node->point.y - intersection.y;
	auto dx = node->point.x - intersection.x;
	auto rad = std::sqrt(dx * dx + dy * dy);

	if ((intersection.y - rad) > sweepY) { return; }

	Vector2D tempPoint(intersection.x, intersection.y - rad);

	auto circleEvent = new tuple(tempPoint, false, *node);


	node->circle = circleEvent;
	_data.queue.insert(circleEvent);
}


VoronoiData2::Node* Voronoi2::getNodeOverPoint(const Vector2D& point) {
	auto* siteNode = &_data.beachline;
	auto edgeNode = siteNode->next;
	double x = 0;
	//遍历链表
	while (edgeNode && edgeNode->next) {
		if (edgeNode->edge.dir.x == 0) {
			cout << "error 1" << endl;
		}

		auto parabola = getEquationOfParabola(siteNode->point, sweepY);

		auto A = std::get<0>(parabola);
		auto B = std::get<1>(parabola) - edgeNode->edge.m;
		auto C = std::get<2>(parabola) - edgeNode->edge.c;

		auto discriminant = (B * B) - (4 * A * C);
		auto X1 = ((-1 * B) + std::sqrt(discriminant)) / (2 * A);
		auto X2 = ((-1 * B) - std::sqrt(discriminant)) / (2 * A);

		auto min = X1 < X2 ? X1 : X2;
		auto max = X1 > X2 ? X1 : X2;

		x = edgeNode->edge.dir.x < 0 ? min : max;
		if (point.x == x)cout << "warning!" << endl;

		if (point.x < x) {
			return siteNode;
		}

		siteNode = edgeNode->next;
		edgeNode = siteNode->next;
	}
	return siteNode;
}


tuple<double, double, double> Voronoi2::getEquationOfParabola(Vector2D point, double sweepY) {
	auto k = (point.y + sweepY) / 2.0;
	auto p = (point.y - sweepY) / 2.0;
	auto A = 1 / (4.0 * p);
	auto B = (-1 * point.x) / (2.0 * p);
	auto C = ((point.x * point.x) / (4.0 * p)) + k;
	tuple<double, double, double> result(A, B, C);
	return result;
}

void Voronoi2::removeEvent(tuple<Vector2D, bool, VoronoiData2::Node>* event) {
	auto eNode = std::get<2>(*event);
	VoronoiData2::Edge* leftEdge;
	VoronoiData2::Edge* rightEdge;
	leftEdge = nullptr;
	rightEdge = nullptr;
	if (eNode.prev)
		leftEdge = &eNode.prev->edge;
	if (eNode.next)
		rightEdge = &eNode.next->edge;

	auto intersection = getEdgeIntersection(leftEdge, rightEdge);
	if (intersection.x == error_interSection.x) {
		return;
	}
	if (leftEdge) {
		leftEdge->end = intersection;
	}
	if (rightEdge) {
		rightEdge->end = intersection;
	}


	_data.edges.push_back(leftEdge);
	_data.edges.push_back(rightEdge);

	VoronoiData2::Node* leftSite = nullptr;
	VoronoiData2::Node* rightSite = nullptr;
	if (eNode.prev)
		leftSite = eNode.prev->prev;
	if (eNode.next)
		rightSite = eNode.next->next;

	if (leftSite && rightSite) {
		auto newNode = new VoronoiData2::Node;

		VoronoiData2::Edge tempE(intersection, leftSite->point, rightSite->point);
		variant<Vector2D, VoronoiData2::Edge> tempE1 = tempE;
		newNode->set(tempE1, 1);

		newNode->setNext(rightSite);
		leftSite->setNext(newNode);

		if (leftSite->circle)
		{
			_data.queue.removeEvent(leftSite->circle);
			leftSite->circle = nullptr;
		}
		if (rightSite->circle)
		{
			_data.queue.removeEvent(rightSite->circle);
			rightSite->circle = nullptr;
		}

		circleCheck(newNode->prev);
		circleCheck(newNode->next);
	}
}

Vector2D Voronoi2::getEdgeIntersection(const VoronoiData2::Edge* left, const VoronoiData2::Edge* right) {

	if (!left || !right) { return error_interSection; }
	if (left->m == right->m) { return error_interSection; }
	auto x = (right->c - left->c) / (left->m - right->m);
	auto y = (left->m * right->c - right->m * left->c) / (left->m - right->m);

	if (((x - left->start.x) / left->dir.x) < 0) { return error_interSection; }
	if (((y - left->start.y) / left->dir.y) < 0) { return error_interSection; }
	if (((x - right->start.x) / right->dir.x) < 0) { return error_interSection; }
	if (((y - right->start.y) / right->dir.y) < 0) { return error_interSection; }
	return Vector2D(x, y);
}

double Voronoi2::getY(Vector2D& focus, Vector2D& eventpoint) {
	auto p = (focus.y - sweepY) / 2;
	auto h = focus.x;
	auto k = focus.y - p;
	auto a = 1 / (4 * p);
	auto b = (-1 * h) / (2 * p);
	auto c = ((h * h) / (4 * p)) + k;
	return a * eventpoint.x * eventpoint.x + b * eventpoint.x + c;
}