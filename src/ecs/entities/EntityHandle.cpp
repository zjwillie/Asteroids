#include "EntityHandle.hpp"

bool EntityHandle::isValid() const {
	return generation != 0;
}