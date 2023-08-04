#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "../ecs.h"
#include "../components/box_collider_component.h"
#include "../../event_bus/event_bus.h"
#include "../../events/collision_event.h"

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
		event.a.kill();
		event.b.kill();
	}

	void update()
	{
		
	}
};

#endif // !DAMAGE_SYSTEM_H
