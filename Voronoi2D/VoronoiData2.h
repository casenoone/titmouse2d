#ifndef VORONOIDATA2_H
#define VORONOIDATA2_H

#include <iostream>
using namespace std;
#include <variant>
#include <list>
#include <tuple>

#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Array.hpp"
#include "../titmouse2d/src/MathUtils.hpp"

//我擦，这里暂时不完全实现
class VoronoiData2 {
public:
	struct Edge {
	public:
		Edge() = default;

		Edge(Vector2D start_, Vector2D leftPoint, Vector2D rightPoint) {
			start = start_;
			end = Vector2D::zero();
			m = -1.0 / ((leftPoint.y - rightPoint.y) / (leftPoint.x - rightPoint.x));
			c = start.y - m * start.x;
			dir.x = -1 * (leftPoint.y - rightPoint.y);
			dir.y = leftPoint.x - rightPoint.x;
			adjacent = nullptr;
		}

		void addAdjacent(Edge* edge) {
			adjacent = edge;
		}

	public:
		Vector2D start;
		Vector2D end;
		double m;
		double c;
		Vector2D dir;
		Edge* adjacent;
	};

	//链表的node
	struct Node {
	public:
		Node() {

		}

		Node(variant<Vector2D, Edge>& data_, int type) {
			if (type == 0) {
				point = std::get<0>(data_);
				type = 0;
			}
			else {
				edge = std::get<1>(data_);
				type = 1;
			}

		}

		void set(variant<Vector2D, Edge>& data_, int type) {
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


	public:
		Vector2D point;
		Edge edge;
		int type;
		tuple<Vector2D, bool, Node>* circle = nullptr;

		Node* next = nullptr;
		Node* prev = nullptr;
	};

	struct Queue {
	public:
		Queue() = default;

		void insert(tuple<Vector2D, bool, Node>* E) {
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

		tuple<Vector2D, bool, Node>* maxQ() {
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
		void removeEvent(tuple<Vector2D, bool, Node>* E) {
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
		list<tuple<Vector2D, bool, Node>*> queueList;
	};

	VoronoiData2() = default;

public:

	//需要写析构函数把它解构掉
	Queue queue;
	Array<Vector2D> sites;
	VoronoiData2::Node beachline;
	vector<VoronoiData2::Edge*> edges;
};





#endif