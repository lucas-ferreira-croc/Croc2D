#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../ecs.h"
#include "../../event_bus/event_bus.h"
#include "../../events/key_pressed_event.h"
#include "../components/transform.h"
#include "../components/rigidbody_component.h"

#include <SDL.h>
class KeyboardMovementSystem : public System
{
public:
	KeyboardMovementSystem()
	{
		require_component<TransformComponent>();
		require_component<RigidBodyComponent>();
	}

	void subscribe_to_events(std::unique_ptr<EventBus>& event_bus)
	{
		event_bus->subscribe_to_event<KeyPressedEvent>(this, &KeyboardMovementSystem::on_key_pressed);
	}

	void on_key_pressed(KeyPressedEvent& event)
	{
		const char* key_name = SDL_GetKeyName(event.key_pressed);
		std::cout << "pressed key = " << key_name << std::endl;
	}

	void update()
	{
	
	}
};

#endif // !KEYBOARD_MOVEMENT_SYSTEM_H

