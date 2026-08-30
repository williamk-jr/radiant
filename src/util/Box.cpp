#include "radiant/util/Box.h"

namespace Radiant {

	Box::Box() : minX(0), minY(0), maxX(0), maxY(0) {}

	Box::Box(float minX, float minY, float maxX, float maxY) : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

	float Box::getMinX() {
		return this->minX;
	}

	float Box::getMinY() {
		return this->minY;
	}

	float Box::getMaxX() {
		return this->maxX;
	}

	float Box::getMaxY() {
		return this->maxY;
	}

	bool Box::containsX(Box box) {
		return this->getMaxX() >= box.getMaxX() && this->getMinX() <= box.getMinX();
	}

	bool Box::containsY(Box box) {
		return this->getMaxY() >= box.getMaxY() && this->getMinY() <= box.getMinY();
	}

	bool Box::contains(Box box) {
		return this->containsX(box) && this->containsY(box);
	}

} // namespace Radiant
