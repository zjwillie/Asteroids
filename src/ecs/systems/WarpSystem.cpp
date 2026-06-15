#include "WarpSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityHandle.hpp"
#include "../components/Transform.hpp"

void WarpSystem::update(WorldContext& worldContext, float) {
	auto& transforms = worldContext.getEntityManager().getTransforms();

	for (auto& [index, transform] : transforms.getAll()) {

		// check if entity is in the window bounds + margin_
		if (transform.position.x > playAreaWidth_ + margin_) {
			transform.position.x = -margin_;
		}

		if (transform.position.x < -margin_) {
			transform.position.x = playAreaWidth_ + margin_;
		}

		if (transform.position.y > playAreaHeight_ + margin_) {
			transform.position.y = -margin_;
		}

		if (transform.position.y < -margin_) {
			transform.position.y = playAreaHeight_ + margin_;
		}
	}
};