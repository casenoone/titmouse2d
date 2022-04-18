#ifndef VORONOIDATA2_H
#define VORONOIDATA2_H

#include <iostream>
#include <variant>
#include <list>
#include <tuple>


#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Array.hpp"
#include "../titmouse2d/src/MathUtils.hpp"

class VoronoiData2 {
public:
	struct Edge {
	public:
		Edge() = default;

		Edge(Vector2D start_, Vector2D leftPoint_, Vector2D rightPoint_) {
			start = start_;
			end = Vector2D::zero();
			m = -1.0 / ((leftPoint_.y - rightPoint_.y) / (leftPoint_.x - rightPoint_.x));
			c = start.y - m * start.x;
			dir.x = -1 * (leftPoint_.y - rightPoint_.y);
			dir.y = leftPoint_.x - rightPoint_.x;
			adjacent = nullptr;

		}

		void addAdjacent(Edge* edge) {
			adjacent = edge;
		}

		void setIndex(int i, int j) {
			leftIndex = i;
			rightIndex = j;
		}

	public:
		Vector2D start;
		Vector2D end;
		double m;
		double c;
		Vector2D dir;
		Edge* adjacent;

		int leftIndex = 0;
		int rightIndex = 0;

		~Edge() {
			adjacent = nullptr;
		}

	};

	//链表的node
	struct Node {
	public:
		Node() {

		}

		Node(std::variant<Vector2D, VoronoiData2::Edge*>& data_, int type) {
			if (type == 0) {
				point = std::get<0>(data_);
				type = 0;
			}
			else {
				edge = std::get<1>(data_);
				type = 1;
			}

		}

		void set(std::variant<Vector2D, VoronoiData2::Edge*>& data_, int type) {
			if (type == 0) {
				point = std::get<0>(data_);
				type = 0;
			}
			else {
				edge = std::get<1>(data_);
				type = 1;
			}

		}

		void setNext(Node* N) {
			if (N) {
				N->prev = this;
			}
			this->next = N;
		}

		void setIndex(int i) {
			index = i;
		}

		~Node() {
			//delete this;
		}

	public:
		Vector2D point;
		Edge* edge;
		int type;
		std::shared_ptr<std::tuple<Vector2D, bool, Node>> circle;

		int index = 0;

		Node* next = nullptr;
		Node* prev = nullptr;
	};

	struct Queue {
	public:
		Queue() = default;

		void insert(std::shared_ptr<std::tuple<Vector2D, bool, Node>> E) {
			auto EPoint = std::get<0>(*E);


			if (queueList.empty()) {
				queueList.push_back(E);
			}
			else {
				for (auto iter = queueList.begin(); iter != queueList.end(); ++iter) {

					auto temp = iter;

					auto point = std::get<0>(**iter);
					if (point.y < EPoint.y) {
						iter = queueList.insert(iter, E);
						break;
					}

					else if (point.y == EPoint.y) {
						if (point.x < EPoint.x) {
							iter = queueList.insert(iter, E);
							break;
						}
					}


					else if ((++temp) == queueList.end()) {
						queueList.push_back(E);
						break;
					}
				}
			}
		}

		std::shared_ptr < std::tuple<Vector2D, bool, Node>> maxQ() {
			auto R = queueList.front();
			queueList.pop_front();
			return R;
		}

		size_t count() {
			return queueList.size();
		}

		bool notEmpty() {
			return !queueList.empty();
		}

		//注意迭代器失效问题
		//可以确保这里没有问题
		void removeEvent(std::shared_ptr<std::tuple<Vector2D, bool, Node>> E) {
			if (!notEmpty())return;
			for (auto iter = queueList.begin(); iter != queueList.end(); ++iter) {

				if ((*iter) == E) {

					iter = queueList.erase(iter);
				}
				if (iter == queueList.end()) {
					break;
				}
			}
		}


	public:
		std::list<std::shared_ptr<std::tuple<Vector2D, bool, Node>>> queueList;
	};

	VoronoiData2() = default;

	//需要把queueList的动态内存全部释放掉
	//Node申请的动态内存也要释放掉
	//=除node以外，其他全部地方使用shared_ptr
	~VoronoiData2() {

		/*auto p = beachline;
		auto pprev = beachline->prev;
		while (p) {
			if (p) {
				auto q = p;
				p = p->next;
				delete q;
			}
		}

		auto p1 = pprev;
		while (p1) {
			auto q = p1;
			p1 = p1->prev;
			delete q;
		}*/

		/*for (auto i = edges.begin(); i != edges.end(); ++i) {
			if (*i) {
				delete (*i);
				*i = nullptr;
			}
		}*/


		for (auto i = fuckList.begin(); i != fuckList.end(); ++i) {
			if (*i) {
				delete (*i);
				*i = nullptr;
			}
		}

		for (auto i = fuckList1.begin(); i != fuckList1.end(); ++i) {
			if (*i) {
				delete (*i);
				*i = nullptr;
			}
		}

	}

public:
	Queue queue;
	Array<Vector2D> sites;
	VoronoiData2::Node* beachline;
	std::vector<VoronoiData2::Edge*> edges;
	std::vector<Edge*> fuckList;
	std::vector<Node*> fuckList1;
};





#endif