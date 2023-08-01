#ifndef DEBUG_SYSTEM_H
#define DEBUG_SYSTEM_H

#include <SDL.h>

#include "../ecs.h"
#include "../components/transform.h"
#include "../components/box_collider_component.h"


class DebugSystem : public System
{
public:
	DebugSystem()
	{
		require_component<TransformComponent>();
		require_component<BoxColliderComponent>();

	}

	void update(SDL_Renderer* renderer)
	{
		for(Entity entity : get_system_entities())
		{
			auto transform = entity.get_component<TransformComponent>();
			auto box_collider = entity.get_component<BoxColliderComponent>();

			SDL_Rect rect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(box_collider.width * transform.scale.x),
				static_cast<int>(box_collider.height * transform.scale.y)
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0.0f);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
};

#endif // !DEBUG_SYSTEM_H
