#include "pch.h"

#include "ShipControlSystem.hpp"

#include "../../input/InputAction.hpp"
#include "../../core/WorldContext.hpp"
#include "../entities/EntityManager.hpp"
#include "../../input/InputManager.hpp"
#include "../../events/EventManager.hpp"
#include "../../utilities/logger/Logger.hpp"

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
				// also they have to be seperated so we can do more than one thing at a time
				// SDL_Scancode thrust = SDL_SCANCODE_UP;
				if (worldContext.getInputManager().isDown(shipBindings_.thrust)) {
					// KEY_DOWN block, after updating state:

					Vec2 facing = { std::cos(transform->rotation), std::sin(transform->rotation) };

					velocity->linearVelocity += facing * thrustForce_ * deltaTime;
				}

				// reset angular velocity
				velocity->angularVelocity = 0.0f;
				// SDL_Scancode rotateLeft = SDL_SCANCODE_LEFT;
				if (worldContext.getInputManager().isDown(shipBindings_.rotateLeft)) {
					// KEY_DOWN block, after updating state:
					velocity->angularVelocity = -turnSpeed_;
				}

				// SDL_Scancode rotateRight = SDL_SCANCODE_RIGHT;
				if (worldContext.getInputManager().isDown(shipBindings_.rotateRight)) {
					// KEY_DOWN block, after updating state:
					velocity->angularVelocity = turnSpeed_;
				}

				if (worldContext.getInputManager().isDown(shipBindings_.rotateRight) && worldContext.getInputManager().isDown(shipBindings_.rotateLeft)) {
					velocity->angularVelocity = 0.0f;
				}


				// SDL_Scancode fire = SDL_SCANCODE_SPACE;
				if (worldContext.getInputManager().wasPressed(shipBindings_.fire)) {
					// KEY_DOWN block, after updating state:
					FireRequestedEvent fireRequest{};
					Vec2 facing = { std::cos(transform->rotation), std::sin(transform->rotation) };
					fireRequest.position = transform->position + facing * 8.0f; // nose offset
					fireRequest.direction = transform->rotation;
					fireRequest.speedDifferential = velocity->linearVelocity.length();

					LOG_DEBUG("Ship", "Fire emitted");
					worldContext.getEventManager().emit(fireRequest);
				}

				// Drag every frame
				velocity->linearVelocity *= (1.0f - drag_ * deltaTime);

				if (velocity->linearVelocity.length() > maxSpeed_) {
					velocity->linearVelocity = velocity->linearVelocity.normalized() * maxSpeed_;
				}
			}

		}
	}
}