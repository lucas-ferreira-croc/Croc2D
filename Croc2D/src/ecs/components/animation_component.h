#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <SDL.h>

struct AnimationComponent
{
	int num_frames;
	int current_frame;
	int frame_rate_speed;
	bool is_loop;
	int start_time;

	AnimationComponent(int num_frames = 1, int frame_rate_speed = 1, bool is_loop = true)
		: num_frames(num_frames), current_frame(1), frame_rate_speed(frame_rate_speed), is_loop(is_loop), start_time(SDL_GetTicks())
	{
		
	}
};
#endif // !ANIMATION_COMPONENT_H
