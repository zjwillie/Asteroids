#include "EntityManager.hpp"

EntityHandle EntityManager::create() {
	EntityHandle newEntity;
	// check if freeList_ has any old slots we can use
	if (!freeList_.empty()) {
		// pop off the top and use that one
		newEntity.index = freeList_.back();
		newEntity.generation = generations_[newEntity.index];
	}
	else {
		// create an all new baby
		newEntity.index = generations_.size();
		newEntity.generation = 1;
		generations_.push_back(1);
	}

	return newEntity;
}

void EntityManager::destroy(const EntityHandle& handle) {
	if (isAlive(handle)) {
		// it's dead Jim, we need a new gen
		++generations_[handle.index];
		// put it on the pile
		freeList_.push_back(handle.index);
	}
	// might need to throw an error or seomthing here if nothing is destoryed, or a warn
}

bool EntityManager::isAlive(const EntityHandle& handle) {
	// check out of range
	if (handle.index >= generations_.size()) return false;
	// ensure handles.generation matches generation_ at that index
	if (handle.generation != generations_[handle.index]) return false;

	// probably should log something when this fails... maybe not here
	return true;
}