#pragma once

#include <vector>

#include "FireRequestedEvent.hpp"
#include "BulletHitAsteroidEvent.hpp"
#include "ShipHitAsteroidEvent.hpp"

class EventManager {
public:
    void clearFrame();

    void emit(const FireRequestedEvent& event);
    const std::vector<FireRequestedEvent>& getFireRequests() const;

    void emit(const BulletHitAsteroidEvent& event);
    const std::vector<BulletHitAsteroidEvent>& getBulletHitAsteroidEvents() const;

    void emit(const ShipHitAsteroidEvent& event);
    const std::vector<ShipHitAsteroidEvent>& getShipHitAsteroidEvents() const;

private:
    std::vector<FireRequestedEvent> fireRequestedEvents_;
    std::vector<BulletHitAsteroidEvent> bulletHitAsteroidEvents_;
    std::vector<ShipHitAsteroidEvent> shipHitAsteroidEvents_;
};