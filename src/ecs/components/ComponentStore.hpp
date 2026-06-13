// Storage is intentionally hidden behind the interface.
// Current implementation: unordered_map (simple, sufficient for small entity counts)
// Future: replace with archetype/sparse-set storage without changing callers.

#pragma once

#include <cstdint>
#include <unordered_map>

#include "../entities/EntityHandle.hpp"

template <typename T>
class ComponentStore {
public:
	void add(const EntityHandle& entity, T component);
	void remove(const EntityHandle& entity);
	T* get(const EntityHandle& entity);

	std::unordered_map<uint32_t, T>& getAll();
	const std::unordered_map<uint32_t, T>& getAll() const;
private:
	// data_ must remain private to update archtypes as needed for complexity
	std::unordered_map<uint32_t, T> data_;
};

template <typename T>
std::unordered_map<uint32_t, T>& ComponentStore<T>::getAll() {
	return data_;
}

template <typename T>
const std::unordered_map<uint32_t, T>& ComponentStore<T>::getAll() const {
	return data_;
}

template <typename T>
void ComponentStore<T>::add(const EntityHandle& entity, T component) {
	if (entity.isValid()) {
		data_.insert({entity.index, component});
	}
	else {
		// log entity is not valid!
	}
	// maybe should be bool so we can see if this was valid, or flag or something here
}

template <typename T>
T* ComponentStore<T>::get(const EntityHandle& entity) {
	auto it = data_.find(entity.index);
	if (it == data_.end()) return nullptr;
	return &it->second;
}

template <typename T>
void ComponentStore<T>::remove(const EntityHandle& entity) {
	data_.erase(entity.index);
}