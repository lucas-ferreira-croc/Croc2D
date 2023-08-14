#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ecs.h"
#include "../components/box_collider_component.h"
#include "../components/projectile_component.h"
#include "../components/health_component.h"
#include "../../event_bus/event_bus.h"
#include "../../events/collision_event.h"
#include "../../logger/logger.h"

class DamageSystem : public System
{
public:
	DamageSystem()
	{
		require_component<BoxColliderComponent>();
	}

	void subscribe_to_events(std::unique_ptr<EventBus>& event_bus)
	{
		event_bus->subscribe_to_event<CollisionEvent>(this, &DamageSystem::on_collision);
	}

	void on_collision(CollisionEvent& event)
	{
		Entity a = event.a;
		Entity b = event.b;

		Logger::log("algo colidiu");

		if(a.belongs_to_group("enemies") && a.has_tag("player")
			||
		   b.has_tag("player") && a.belongs_to_group("enemies"))
		{
			return;
		}

		if(a.belongs_to_group("projectiles") && b.has_tag("player"))
		{
			on_projectile_hits_player(a, b);
		}
		
		if(b.belongs_to_group("projectiles") && a.has_tag("player"))
		{
			on_projectile_hits_player(b, a);

		}

		
		if (a.belongs_to_group("projectiles") && b.belongs_to_group("enemies"))
		{
			on_projectile_hits_enemy(a, b);
		}

		if (b.belongs_to_group("projectiles") && a.belongs_to_group("enemies"))
		{
			on_projectile_hits_enemy(b, a);
		}
		
	}

	void update()
	{
		
	}

	void on_projectile_hits_player(Entity projectile, Entity player)
	{
		const auto projectile_component = projectile.get_component<ProjectileComponent>();

		if(!projectile_component.is_friendly)
		{
			auto& health = player.get_component<HealthComponent>();
			health.health_percentage -= projectile_component.hit_percentage;

			if(health.health_percentage <= 0 )
			{
				player.kill();
			}
		}

		projectile.kill();
	}

	void on_projectile_hits_enemy(Entity projectile, Entity enemey)
	{
		const auto projectile_component = projectile.get_component<ProjectileComponent>();

		if (projectile_component.is_friendly)
		{
			auto& health = enemey.get_component<HealthComponent>();
			health.health_percentage -= projectile_component.hit_percentage;

			if (health.health_percentage <= 0)
			{
				enemey.kill();
			}
		}

		projectile.kill();
	}
};

#endif // !DAMAGE_SYSTEM_H
