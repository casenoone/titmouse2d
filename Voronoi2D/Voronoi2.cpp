#include "Voronoi2.h"
#include "../titmouse2d/src/random.h"

static const Vector2D error_interSection(-1000, 0);

static double sweepY = 0;

void Voronoi2::generateVoronoi(int number, double width, double height) {
	/*auto& queue = _data.queue;
	for (auto i = 0; i < number; ++i) {
		auto tempX = random_double(0, 2);
		auto tempY = random_double(0, 2);
		Vector2D point(tempX, tempY);

		VoronoiData2::Node node;
		node.setIndex(i);
		auto e = std::make_shared < std::tuple<Vector2D, bool, VoronoiData2::Node>>(point, true, node);
		queue.insert(e);
	}

	std::variant<Vector2D, VoronoiData2::Edge*> max_Point = std::get<0>(*queue.maxQ());
	_data.beachline->set(max_Point, 0);

	while (_data.queue.notEmpty()) {

		auto event = queue.maxQ();
		Vector2D ePoint = std::get<0>(*event);
		sweepY = ePoint.y;
		bool isSiteEvent = std::get<1>(*event);
		int index = std::get<2>(*event).index;
		if (isSiteEvent) { addEvent(*event, index); }
		else {
			removeEvent(event);
		}
	}*/


}


void Voronoi2::generateVoronoi(const Array<Vector2D>& positions) {
	auto& queue = _data.queue;
	auto len = positions.dataSize();
	for (auto i = 0; i < len; ++i) {
		double tempX = positions.lookAt(i).x;
		double tempY = positions.lookAt(i).y;
		Vector2D point(tempX, tempY);

		VoronoiData2::Node node;
		node.setIndex(i);
		auto e = std::make_shared < std::tuple<Vector2D, bool, VoronoiData2::Node>>(point, true, node);
		queue.insert(e);
	}
	auto p = queue.maxQ();
	std::variant<Vector2D, VoronoiData2::Edge*> max_Point = std::get<0>(*p);
	_data.beachline = new VoronoiData2::Node;
	_data.beachline->set(max_Point, 0);
	_data.beachline->setIndex(std::get<2>(*p).index);
	_data.fuckList1.push_back(_data.beachline);

	while (_data.queue.notEmpty()) {
		auto event = queue.maxQ();
		Vector2D ePoint = std::get<0>(*event);
		sweepY = ePoint.y;
		bool isSiteEvent = std::get<1>(*event);
		int index = std::get<2>(*event).index;
		if (isSiteEvent) { addEvent(*event, index); }
		else {
			removeEvent(event);
		}
	}


}


//可以在这里增加一个参数，表明是event附带的粒子索引
void Voronoi2::addEvent(std::tuple<Vector2D, bool, VoronoiData2::Node>& event, int index) {
	auto ePoint = std::get<0>(event);
	_data.sites.push(ePoint);

	std::variant<Vector2D, VoronoiData2::Edge*> eepoint = ePoint;
	VoronoiData2::Node* newNode = new VoronoiData2::Node(eepoint, 0);
	_data.fuckList1.push_back(newNode);
	newNode->setIndex(index);

	auto node = getNodeOverPoint(ePoint);
	if (node) {
		Vector2D start(ePoint.x, getY(node->point, ePoint));

		//这里必须传入粒子的索引
		auto rightEdge = new VoronoiData2::Edge(start, ePoint, node->point);
		auto leftEdge = new VoronoiData2::Edge(start, node->point, ePoint);
		_data.fuckList.push_back(rightEdge);
		_data.fuckList.push_back(leftEdge);
		rightEdge->setIndex(index, node->index);
		leftEdge->setIndex(node->index, index);

		//曹他妈的这里循环引用
		leftEdge->addAdjacent(rightEdge);
		rightEdge->addAdjacent(leftEdge);

		std::variant<Vector2D, VoronoiData2::Edge*> left = leftEdge;
		std::variant<Vector2D, VoronoiData2::Edge*> right = rightEdge;
		auto leftNode = new VoronoiData2::Node(left, 1);
		auto rightNode = new VoronoiData2::Node(right, 1);
		_data.fuckList1.push_back(leftNode);
		_data.fuckList1.push_back(rightNode);
		std::variant<Vector2D, VoronoiData2::Edge*> nnpoint = node->point;
		auto duplicateNode = new VoronoiData2::Node(nnpoint, 0);
		_data.fuckList1.push_back(duplicateNode);
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
}

void Voronoi2::circleCheck(VoronoiData2::Node* node) {
	VoronoiData2::Edge* leftEdge = nullptr;
	VoronoiData2::Edge* rightEdge = nullptr;
	if (node->prev)
		leftEdge = node->prev->edge;
	if (node->next)
		rightEdge = node->next->edge;

	auto intersection = getEdgeIntersection(leftEdge, rightEdge);
	if (intersection.x == -error_interSection.x) {
		return;
	}

	auto dy = node->point.y - intersection.y;
	auto dx = node->point.x - intersection.x;
	auto rad = std::sqrt(dx * dx + dy * dy);

	if ((intersection.y - rad) > sweepY) { return; }

	Vector2D tempPoint(intersection.x, intersection.y - rad);

	auto circleEvent = std::make_shared < std::tuple<Vector2D, bool, VoronoiData2::Node>>(tempPoint, false, *node);


	node->circle = circleEvent;
	_data.queue.insert(circleEvent);
}


VoronoiData2::Node* Voronoi2::getNodeOverPoint(const Vector2D& point) {
	auto siteNode = _data.beachline;
	auto edgeNode = siteNode->next;
	double x = 0;
	//遍历链表
	while (edgeNode && edgeNode->next) {
		if (edgeNode->edge->dir.x == 0) {
			std::cout << "error 1" << std::endl;
		}

		auto parabola = getEquationOfParabola(siteNode->point, sweepY);

		auto A = std::get<0>(parabola);
		auto B = std::get<1>(parabola) - edgeNode->edge->m;
		auto C = std::get<2>(parabola) - edgeNode->edge->c;

		auto discriminant = (B * B) - (4 * A * C);
		auto X1 = ((-1 * B) + std::sqrt(discriminant)) / (2 * A);
		auto X2 = ((-1 * B) - std::sqrt(discriminant)) / (2 * A);

		auto min = X1 < X2 ? X1 : X2;
		auto max = X1 > X2 ? X1 : X2;

		x = edgeNode->edge->dir.x < 0 ? min : max;
		if (point.x == x)std::cout << "warning!" << std::endl;

		if (point.x < x) {
			return siteNode;
		}

		siteNode = edgeNode->next;
		edgeNode = siteNode->next;
	}
	return siteNode;
}


std::tuple<double, double, double> Voronoi2::getEquationOfParabola(Vector2D point, double sweepY) {
	auto k = (point.y + sweepY) / 2.0;
	auto p = (point.y - sweepY) / 2.0;
	auto A = 1 / (4.0 * p);
	auto B = (-1 * point.x) / (2.0 * p);
	auto C = ((point.x * point.x) / (4.0 * p)) + k;
	std::tuple<double, double, double> result(A, B, C);
	return result;
}

void Voronoi2::removeEvent(std::shared_ptr<std::tuple<Vector2D, bool, VoronoiData2::Node>> event) {
	auto eNode = std::get<2>(*event);
	VoronoiData2::Edge* leftEdge = nullptr;
	VoronoiData2::Edge* rightEdge = nullptr;
	if (eNode.prev)
		leftEdge = eNode.prev->edge;
	if (eNode.next)
		rightEdge = eNode.next->edge;

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
		_data.fuckList1.push_back(newNode);
		auto  tempE = new VoronoiData2::Edge(intersection, leftSite->point, rightSite->point);
		//这里是漏网之鱼
		_data.fuckList.push_back(tempE);

		tempE->setIndex(leftSite->index, rightSite->index);
		std::variant<Vector2D, VoronoiData2::Edge*> tempE1 = tempE;
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