#ifndef PLAYERSHOOTEVENT_H
#define PLAYERSHOOTEVENT_H

#include "../event_bus/event.h"
#include "../ecs/ecs.h"

class PlayerShootEvent : public Event
{
public:
	std::unique_ptr<Registry>& registry;

	PlayerShootEvent(std::unique_ptr<Registry>& registry) : registry(registry)
	
	{
	}
};

#endif
