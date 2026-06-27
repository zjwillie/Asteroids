#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "DronNode.hpp"   // DronMap, DronNode

class EntityManager;
struct EntityHandle;

// Maps a .ent/section name (e.g. "Transform") to a loader that reads that
// section's DronMap and attaches one component to an entity. Sections with no
// registered loader (e.g. [Meta], [Physics]) are skipped — there is no entity-type
// branching anywhere; the section name itself selects the loader.
class ComponentFactory {
public:
    ComponentFactory();

    // Attach the component for `section` to `handle`, reading from `data`.
    // Returns false (and does nothing) if no loader is registered for `section`.
    bool load(const std::string& section,
              EntityManager& entityManager,
              const EntityHandle& handle,
              const DronMap& data) const;

    bool has(const std::string& section) const;

private:
    using Loader = std::function<void(EntityManager&, const EntityHandle&, const DronMap&)>;
    std::unordered_map<std::string, Loader> loaders_;
};
