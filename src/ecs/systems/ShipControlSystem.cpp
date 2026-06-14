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
				// also they have to be seperated so we can do more than one thing at a time
				// SDL_Scancode thrust = SDL_SCANCODE_UP;
				if (worldContext.getInputManager().isDown(shipBindings_.thrust)) {
					// KEY_DOWN block, after updating state:

					Vec2 facing = { std::cos(transform->rotation), std::sin(transform->rotation) };

					/*
					SDL_Log("WE HAVE F THRUST!");
					SDL_Log("velocity: %f %f", velocity->linearVelocity.x, velocity->linearVelocity.y);
					SDL_Log("thrustForce: %f", thrustForce_);
					SDL_Log("deltaTime: %f", deltaTime);
					SDL_Log("facing: %f %f", facing.x, facing.y);
					*/

					velocity->linearVelocity += facing * thrustForce_ * deltaTime;
				}

				// reset angular velocity
				velocity->angularVelocity = 0.0f;
				// SDL_Scancode rotateLeft = SDL_SCANCODE_LEFT;
				if (worldContext.getInputManager().isDown(shipBindings_.rotateLeft)) {
					// KEY_DOWN block, after updating state:
					SDL_Log("WE HAVE L THRUST!");
					velocity->angularVelocity = -turnSpeed_;
				}

				// SDL_Scancode rotateRight = SDL_SCANCODE_RIGHT;
				if (worldContext.getInputManager().isDown(shipBindings_.rotateRight)) {
					// KEY_DOWN block, after updating state:
					SDL_Log("WE HAVE R THRUST!");
					velocity->angularVelocity = turnSpeed_;
				}

				if (worldContext.getInputManager().isDown(shipBindings_.rotateRight) && worldContext.getInputManager().isDown(shipBindings_.rotateLeft)) {
					velocity->angularVelocity = 0.0f;
				}


				// SDL_Scancode fire = SDL_SCANCODE_SPACE;
				if (worldContext.getInputManager().isDown(shipBindings_.fire)) {
					// KEY_DOWN block, after updating state:
					SDL_Log("WE HAVE BLASTERS! Pew pew");
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