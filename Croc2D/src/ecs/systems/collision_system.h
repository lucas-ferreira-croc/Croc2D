#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ecs.h"
#include "../components/box_collider_component.h"
#include "../components/transform.h"
#include "../../logger/logger.h"
#include "../../event_bus/event_bus.h"
#include "../../events/collision_event.h"

class CollissionSystem : public System
{
public:
	CollissionSystem()
	{
		require_component<TransformComponent>();
		require_component<BoxColliderComponent>();
	}

	void update(std::unique_ptr<EventBus>& event_bus)
	{
		auto entities = get_system_entities();
		auto entities_b = get_system_entities();

		for(auto i = entities.begin(); i != entities.end(); i++)
		{
			Entity a = *i;

			for(auto j = i; j != entities.end(); j++)
			{
				Entity b = *j;

				if (a == b) {
					continue;
				}

				auto transform_a = a.get_component<TransformComponent>();
				auto& box_collider_a = a.get_component<BoxColliderComponent>();


				auto transform_b = b.get_component<TransformComponent>();
				auto& box_collider_b = b.get_component<BoxColliderComponent>();

				bool collision = check_AABB_collision(transform_a.position.x + box_collider_a.offset.x, transform_a.position.y + box_collider_a.offset.y, box_collider_a.height, box_collider_a.width,
					transform_b.position.x + box_collider_a.offset.x, transform_b.position.y + box_collider_b.offset.y, box_collider_b.height, box_collider_b.width);
				
				if (collision)
				{

					Logger::log("Entity widh id = " + std::to_string(a.get_id()) + " collided with entity with id =" + std::to_string(b.get_id()));
					box_collider_a.is_colliding = true;
					box_collider_b.is_colliding = true;
					
					event_bus->emit_event<CollisionEvent>(a, b);
				}
			}
		}
	}

private:
	bool check_AABB_collision(double a_x, double a_y, double a_h, double a_w, double b_x, double b_y, double b_h, double b_w)
	{

		return (
			a_x <= b_x + b_w &&
			a_x + a_w >= b_x &&
			a_y <= b_y + b_h &&
			a_y + a_h >= b_y
		);
	}
};

#endif // !COLLISION_SYSTEM_H

