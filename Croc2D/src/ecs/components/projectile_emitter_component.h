#ifndef PROJECTILEEMMITERCOMPONENT_H
#define PROJECTILEEMMITERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL.h>

struct ProjectileEmitterComponent
{
	glm::vec2 velocity;
	
	int repeat_frequency;
	int projectile_duration;
	int hit_percent_damage;
	int last_emission_time;

	bool is_friendly;



	ProjectileEmitterComponent(glm::vec2 velocity = glm::vec2(0), int repeat_frequency = 0, int projectile_duration = 0, int hit_percent_damage = 0, bool is_friendly = false)
		: velocity(velocity), repeat_frequency(repeat_frequency), projectile_duration(projectile_duration),
		hit_percent_damage(hit_percent_damage), last_emission_time(SDL_GetTicks()), is_friendly(is_friendly)
	{}
};

#endif // !PROJECTILEEMMITERCOMPONENT_H

