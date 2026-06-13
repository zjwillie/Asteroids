#include "RenderSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityHandle.hpp"
#include "../components/Transform.hpp"

void RenderSystem::render(WorldContext& worldContext) {
	auto& transforms = worldContext.getEntityManager().getTransforms();
	auto& sprites = worldContext.getEntityManager().getSprites();

	for (auto& [index, sprite] : sprites.getAll()) {
		EntityHandle handle = worldContext.getEntityManager().getHandle(index);
		Transform* transform = transforms.get(handle);
		if (transform) {
			// draw that bitch
			SDL_FRect rect = {
				transform->position.x,
				transform->position.y,
				static_cast<float>(sprite.width),
				static_cast<float>(sprite.height)
			};
			SDL_SetRenderDrawColor(worldContext.getSDLManager().getRenderer(), sprite.red, sprite.green, sprite.blue, 255);
			SDL_RenderFillRect(worldContext.getSDLManager().getRenderer(), &rect);
		}
	}
}