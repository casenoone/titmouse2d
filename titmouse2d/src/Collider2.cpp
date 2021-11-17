#include "Collider2.h"

Collider2::Collider2() {

}

Collider2::~Collider2() {

}

void Collider2::push(const Surface2Ptr& _surface) {
	_surfaces.push_back(_surface);
}