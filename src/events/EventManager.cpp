#include "pch.h"

#include "EventManager.hpp"

// SpawnEntityEvent
void EventManager::emit(const SpawnEntityEvent& event) {
    spawnEntityEvents_.push_back(event);
}
const std::vector<SpawnEntityEvent>& EventManager::getSpawnRequests() const {
    return spawnEntityEvents_;
}

// FireRequestedEvent
void EventManager::emit(const FireRequestedEvent& event) {
    fireRequestedEvents_.push_back(event);
}
const std::vector<FireRequestedEvent>& EventManager::getFireRequests() const {
    return fireRequestedEvents_;
}

// BulletHitAsteroidEvent
void EventManager::emit(const BulletHitAsteroidEvent& event) {
    bulletHitAsteroidEvents_.push_back(event);
}
const std::vector<BulletHitAsteroidEvent>& EventManager::getBulletHitAsteroidEvents() const {
    return bulletHitAsteroidEvents_;
}

// ShipHitAsteroidEvent
void EventManager::emit(const ShipHitAsteroidEvent& event) {
    shipHitAsteroidEvents_.push_back(event);
}
const std::vector<ShipHitAsteroidEvent>& EventManager::getShipHitAsteroidEvents() const {
    return shipHitAsteroidEvents_;
}

void EventManager::clearFrame() {
    spawnEntityEvents_.clear();
    fireRequestedEvents_.clear();
    bulletHitAsteroidEvents_.clear();
    shipHitAsteroidEvents_.clear();
}