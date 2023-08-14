#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include "../ecs.h"
#include "../components/projectile_emitter_component.h"
#include "../components/transform.h"
#include "../components/rigidbody_component.h"
#include "../components/sprite_component.h"
#include "../components/box_collider_component.h"
#include "../components/projectile_component.h"
#include "../components/camera_follow.h"
#include "../components/keyboard_control.h"

#include "../../event_bus/event_bus.h"
#include "../../events/player_shoot_event.h"

#include <iostream>

class ProjectileEmitSystem : public System
{
public:
	ProjectileEmitSystem()
	{
		require_component<ProjectileEmitterComponent>();
		require_component<TransformComponent>();
	}

	void subscribe_to_event(std::unique_ptr<EventBus>& event_bus)
	{
		event_bus->subscribe_to_event<PlayerShootEvent>(this, &ProjectileEmitSystem::on_key_pressed);
	}

	void on_key_pressed(PlayerShootEvent& event)
	{
		for(auto entity : get_system_entities())
		{
			if (entity.has_component<CameraFollowComponent>())// && entity.has_component<CameraFollowComponent>())
			{
				
				auto& projectile_emitter = entity.get_component<ProjectileEmitterComponent>();
				const auto transform = entity.get_component<TransformComponent>();

				emit_projectile(event.registry, entity, projectile_emitter, transform);
			}
		}
	}

	void update(std::unique_ptr<Registry>& registry)
	{
		for(auto entity : get_system_entities())
		{
			auto& projectile_emitter = entity.get_component<ProjectileEmitterComponent>();
			if (projectile_emitter.repeat_frequency == 0) continue;

			const auto transform = entity.get_component<TransformComponent>();

			if (SDL_GetTicks() - projectile_emitter.last_emission_time > projectile_emitter.repeat_frequency)
			{
				emit_projectile(registry, entity, projectile_emitter, transform);
			}
		}
	}

private:
	void emit_projectile(std::unique_ptr<Registry>& registry, Entity entity, ProjectileEmitterComponent& projectile_emitter, TransformComponent transform)
	{
		glm::vec2 projectile_position = transform.position;
		if (entity.has_component<SpriteComponent>())
		{
			const auto sprite = entity.get_component<SpriteComponent>();
			projectile_position.x += (transform.scale.x * sprite.width / 2);
			projectile_position.y += (transform.scale.y * sprite.height / 2);
		}


		glm::vec2 projectile_velocity = projectile_emitter.velocity;
		if (entity.has_component<RigidBodyComponent>())
		{
			auto rigidbody = entity.get_component<RigidBodyComponent>();

			int direction_x = 0;
			int direction_y = 0;
		
			if (rigidbody.velocity.x > 0) direction_x = +1;
			if (rigidbody.velocity.x < 0) direction_x = -1;
			if (rigidbody.velocity.y > 0) direction_y = +1;
			if (rigidbody.velocity.y < 0) direction_y = -1;

			projectile_velocity.x = projectile_emitter.velocity.x * direction_x;
			projectile_velocity.y = projectile_emitter.velocity.y * direction_y;
		}


		Entity projectile = registry->create_entity();
		projectile.group("projectiles");
		projectile.add_component<TransformComponent>(projectile_position, glm::vec2(1.0f, 1.0f), 0.0f);
		projectile.add_component<RigidBodyComponent>(projectile_velocity);
		projectile.add_component<SpriteComponent>("bullet-image", 4, 4, 4);
		projectile.add_component<BoxColliderComponent>(4, 4);
		projectile.add_component<ProjectileComponent>(projectile_emitter.is_friendly, projectile_emitter.hit_percent_damage, projectile_emitter.projectile_duration);

		projectile_emitter.last_emission_time = SDL_GetTicks();
	}
	
};

#endif