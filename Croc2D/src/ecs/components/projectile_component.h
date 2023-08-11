#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <SDL.h>

struct ProjectileComponent
{
	bool is_friendly;
	int hit_percentage;
	int duration;
	int start_time;

	ProjectileComponent(bool is_friendly = false, int hit_percentage = 0, int duration = 0, int start_time = SDL_GetTicks())
		: is_friendly(is_friendly), hit_percentage(hit_percentage), duration(duration), start_time(start_time)
	{
	}
};

#endif // !PROJECTILECOMPONENT_H

