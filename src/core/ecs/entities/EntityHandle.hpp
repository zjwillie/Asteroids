#pragma once

#include <cstdint>

struct EntityHandle {
	bool isValid() const;
	uint32_t index = 0;
	uint32_t generation = 0;

	bool operator==(const EntityHandle&) const = default;
};

bool EntityHandle::isValid() const {
	return generation != 0;
}