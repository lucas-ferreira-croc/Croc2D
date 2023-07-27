#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ecs.h"
#include "../components/transform.h"
#include "../components/rigidbody_component.h"

class MovementSystem : public System
{
public:
	MovementSystem() 
	{
		require_component<TransformComponent>();
		require_component<RigidBodyComponent>();
	}

	void update(float delta_time)
	{
		for(auto entity : get_system_entities())
		{
			auto& transform = entity.get_component<TransformComponent>();
			const auto rigidbody = entity.get_component<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * delta_time;
			transform.position.y += rigidbody.velocity.y * delta_time;
		}
	}
};

#endif