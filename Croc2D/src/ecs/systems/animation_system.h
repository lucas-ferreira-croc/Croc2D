#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../ecs/ecs.h"
#include <SDL.h>

class AnimationSystem : public System
{
public:
	AnimationSystem()
	{
		require_component<SpriteComponent>();
		require_component<AnimationComponent>();
	}

	void update()
	{
		for(auto entity : get_system_entities())
		{
			auto& animation = entity.get_component<AnimationComponent>();
			auto& sprite = entity.get_component<SpriteComponent>();

			animation.current_frame = ((SDL_GetTicks() - animation.start_time) * animation.frame_rate_speed / 1000) % animation.num_frames;
			sprite.src_rect.x = animation.current_frame * sprite.width;
		}
	}
};

#endif // !ANIMATION_SYSTEM_H

