#include "pch.h"

#include "ComponentFactory.hpp"

#include "entities/EntityManager.hpp"   // EntityManager + all component types
#include "entities/EntityHandle.hpp"

// -----------------------------------------------------------------------------
// Scalar readers — getXOr(map, key, fallback). Same pattern as MECS3, adapted
// for MECS4's DronNode (integers are int64_t). Missing key or wrong category
// returns the fallback; numbers coerce between int64 and double.
// -----------------------------------------------------------------------------
static float getFloatOr(const DronMap& d, const std::string& key, float fallback) {
    auto it = d.find(key);
    if (it == d.end()) return fallback;
    const DronNode& n = it->second;
    if (n.is<double>())  return static_cast<float>(n.as<double>());
    if (n.is<int64_t>()) return static_cast<float>(n.as<int64_t>());
    return fallback;
}

static int getIntOr(const DronMap& d, const std::string& key, int fallback) {
    auto it = d.find(key);
    if (it == d.end()) return fallback;
    const DronNode& n = it->second;
    if (n.is<int64_t>()) return static_cast<int>(n.as<int64_t>());
    if (n.is<double>())  return static_cast<int>(n.as<double>());
    return fallback;
}

static bool getBoolOr(const DronMap& d, const std::string& key, bool fallback) {
    auto it = d.find(key);
    if (it == d.end()) return fallback;
    const DronNode& n = it->second;
    return n.is<bool>() ? n.as<bool>() : fallback;
}

static std::string getStringOr(const DronMap& d, const std::string& key, const std::string& fallback) {
    auto it = d.find(key);
    if (it == d.end()) return fallback;
    const DronNode& n = it->second;
    return n.is<std::string>() ? n.as<std::string>() : fallback;
}

// -----------------------------------------------------------------------------
// Loader registry. Each lambda reads one section's map and attaches one
// component. No entity-type names appear here — just section -> component.
// -----------------------------------------------------------------------------
ComponentFactory::ComponentFactory() {
    loaders_["Transform"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getTransforms().add(h, Transform{
            Vec2{ getFloatOr(d, "x", 0.0f), getFloatOr(d, "y", 0.0f) },
            getFloatOr(d, "rotation", 0.0f)
        });
    };

    loaders_["Velocity"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getVelocities().add(h, Velocity{
            Vec2{ getFloatOr(d, "dx", 0.0f), getFloatOr(d, "dy", 0.0f) },
            0.0f
        });
    };

    loaders_["Sprite"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getSprites().add(h, Sprite{
            getIntOr(d, "width", 0),
            getIntOr(d, "height", 0),
            static_cast<uint8_t>(getIntOr(d, "red", 0)),
            static_cast<uint8_t>(getIntOr(d, "green", 0)),
            static_cast<uint8_t>(getIntOr(d, "blue", 0))
        });
    };

    loaders_["Collider"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getColliders().add(h, Collider{ Circle{ getFloatOr(d, "radius", 0.0f) } });
    };

    loaders_["PlayerControlled"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        PlayerControlled pc;
        pc.currently_controllable = getBoolOr(d, "currently_controllable", true);
        pc.lives                  = getIntOr(d, "lives", 3);
        pc.score                  = getIntOr(d, "score", 0);
        pc.isInvulnerable         = getBoolOr(d, "is_invulnerable", false);
        pc.invulnerableTimer      = getFloatOr(d, "invulnerable_timer", 0.0f);
        em.getPlayerControlled().add(h, pc);
    };

    loaders_["Lifetime"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getLifetimes().add(h, Lifetime{
            getFloatOr(d, "remaining", 0.0f),
            getBoolOr(d, "is_dead", false)
        });
    };

    loaders_["Name"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        em.getNames().add(h, Name{ getStringOr(d, "name", "") });
    };

    loaders_["Asteroid"] = [](EntityManager& em, const EntityHandle& h, const DronMap& d) {
        static const std::unordered_map<std::string, AsteroidSize> SIZES{
            { "SMALL",  AsteroidSize::Small  },
            { "MEDIUM", AsteroidSize::Medium },
            { "LARGE",  AsteroidSize::Large  },
        };
        AsteroidSize size = AsteroidSize::Large;
        auto it = SIZES.find(getStringOr(d, "size", "LARGE"));
        if (it != SIZES.end()) size = it->second;
        em.getAsteroids().add(h, Asteroid{ size });
    };
}

bool ComponentFactory::load(const std::string& section,
                            EntityManager& entityManager,
                            const EntityHandle& handle,
                            const DronMap& data) const {
    auto it = loaders_.find(section);
    if (it == loaders_.end()) return false;
    it->second(entityManager, handle, data);
    return true;
}

bool ComponentFactory::has(const std::string& section) const {
    return loaders_.count(section) > 0;
}
