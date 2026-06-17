// src/ecs/EntityDebug.hpp
#pragma once

#include <format>
#include <string>

#include "EntityHandle.hpp"
#include "EntityManager.hpp"
#include "../components/Name.hpp"

inline std::string describeEntity(const EntityHandle& handle, EntityManager& em) {
    Name* name = em.getNames().get(handle);
    return std::format("Entity {}({}) '{}'",
        handle.index,
        handle.generation,
        name ? name->name : "unnamed"
    );
}