#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../ecs.h"
#include "../../event_bus/event_bus.h"
#include "../../events/key_pressed_event.h"
#include "../components/transform.h"
#include "../components/rigidbody_component.h"
#include "../components/keyboard_control.h"

#include <SDL.h>
class KeyboardMovementSystem : public System
{
public:
	KeyboardMovementSystem()
	{
		require_component<KeyBoardControlledComponent>();
		require_component<TransformComponent>();
		require_component<RigidBodyComponent>();
	}

	void subscribe_to_events(std::unique_ptr<EventBus>& event_bus)
	{
		event_bus->subscribe_to_event<KeyPressedEvent>(this, &KeyboardMovementSystem::on_key_pressed);
	}

	void on_key_pressed(KeyPressedEvent& event)
	{
		for(auto entity : get_system_entities())
		{
			const auto keyboard_control = entity.get_component<KeyBoardControlledComponent>();
			auto& sprite = entity.get_component<SpriteComponent>();
			auto& rigidbody = entity.get_component<RigidBodyComponent>();


			switch(event.key_pressed)
			{
				case SDLK_UP:
					rigidbody.velocity = keyboard_control.up_velocity;
					sprite.src_rect.y = sprite.height * 0;
					break;
				case SDLK_RIGHT:
					rigidbody.velocity = keyboard_control.right_velocity;
					sprite.src_rect.y = sprite.height * 1;
					break;

				case SDLK_DOWN:
					rigidbody.velocity = keyboard_control.down_velocity;
					sprite.src_rect.y = sprite.height * 2;
					break;

				case SDLK_LEFT:
					rigidbody.velocity = keyboard_control.left_velocity;
					sprite.src_rect.y = sprite.height * 3;
					break;

			}
		}
	}

	void update()
	{
	
	}
};

#endif // !KEYBOARD_MOVEMENT_SYSTEM_H

