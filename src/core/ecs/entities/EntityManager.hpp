#pragma once
#include <cstdint>
#include <vector>

#include "EntityHandle.hpp"

class EntityManager {
public:
	EntityHandle create();
	void destroy(const EntityHandle& handle);
	bool isAlive(const EntityHandle& handle);

private:
	std::vector<uint32_t> generations_;
	std::vector<uint32_t> freeList_;
};