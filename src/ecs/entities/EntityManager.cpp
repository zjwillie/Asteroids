#include "EntityManager.hpp"

EntityHandle EntityManager::create() {
	EntityHandle newEntity;
	// check if freeList_ has any old slots we can use
	if (!freeList_.empty()) {
		// pop off the top and use that one
		newEntity.index = freeList_.back();
		freeList_.pop_back();
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

EntityHandle EntityManager::getHandle(uint32_t index) const {
	// check index is in bounds first
	EntityHandle entityHandle{ 0, 0 };
	if (generations_.size() <= index) {
		// log, oh shit why would you do this? this index is dead Jim
		return entityHandle;
	}
	entityHandle.generation = generations_[index];
	entityHandle.index = index;
	return entityHandle;
}

void EntityManager::destroy(const EntityHandle& handle) {
	if (isAlive(handle)) {
		// it's dead Jim, we need a new gen
		++generations_[handle.index];
		// put it on the pile
		freeList_.push_back(handle.index);

		// remove all components
		transforms_.remove(handle);
		velocities_.remove(handle);
		sprites_.remove(handle);
		playerControlled_.remove(handle);
		lifetimes_.remove(handle);
		bullets_.remove(handle);
		names_.remove(handle);
		colliders_.remove(handle);
		asteroids_.remove(handle);
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

ComponentStore<Transform>& EntityManager::getTransforms() {
	return transforms_;
}

ComponentStore<Velocity>& EntityManager::getVelocities() {
	return velocities_;
}

ComponentStore<Sprite>& EntityManager::getSprites() {
	return sprites_;
}

ComponentStore<PlayerControlled>& EntityManager::getPlayerControlled() {
	return playerControlled_;
}

ComponentStore<Lifetime>& EntityManager::getLifetimes() {
	return lifetimes_;
}

ComponentStore<Bullet>& EntityManager::getBullets() {
	return bullets_;
}

ComponentStore<Name>& EntityManager::getNames() {
	return names_;
}

ComponentStore<Collider>& EntityManager::getColliders() {
	return colliders_;
}

ComponentStore<Asteroid>& EntityManager::getAsteroids() {
	return asteroids_;
}