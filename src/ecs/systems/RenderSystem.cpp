#include "pch.h"

#include "RenderSystem.hpp"

#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../platform/SDLManager.hpp"
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

			Vec2 facing = { std::cos(transform->rotation), std::sin(transform->rotation) };
			SDL_FRect nose = {
				transform->position.x + facing.x * (sprite.width / 2.0f),
				transform->position.y + facing.y * (sprite.height / 2.0f),
				3.0f, 3.0f
			};
			SDL_SetRenderDrawColor(worldContext.getSDLManager().getRenderer(), 255, 255, 0, 255); // yellow dot
			SDL_RenderFillRect(worldContext.getSDLManager().getRenderer(), &nose);
		}
	}
}