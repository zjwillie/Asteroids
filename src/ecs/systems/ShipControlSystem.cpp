#include "ShipControlSystem.hpp"

#include "../../input/InputAction.hpp"
#include "../../core/WorldContext.hpp"

void ShipControlSystem::update(WorldContext& worldContext, float deltaTime) {
	auto& playerControlleds = worldContext.getEntityManager().getPlayerControlled();
	auto& transforms = worldContext.getEntityManager().getTransforms();
	auto& velocities = worldContext.getEntityManager().getVelocities();

	for (auto& [index, playerControlled] : playerControlleds.getAll()) {
		if (playerControlled.currently_controllable) {
			EntityHandle handle = worldContext.getEntityManager().getHandle(index);

			Transform* transform = transforms.get(handle);
			Velocity* velocity = velocities.get(handle);

			if (transform && velocity) {
				// check all the keys we are interested in from inside worldcontext from inputmanager
				// SDL_Scancode thrust = SDL_SCANCODE_UP;
				if (worldContext.getInputManager().isDown(shipBindings_.thrust)) {
					// KEY_DOWN block, after updating state:
					SDL_Log("WE HAVE THRUST!");
				}
				// SDL_Scancode rotateLeft = SDL_SCANCODE_LEFT;
				// SDL_Scancode rotateRight = SDL_SCANCODE_RIGHT;
				// SDL_Scancode fire = SDL_SCANCODE_SPACE;
			}

		}
	}
}