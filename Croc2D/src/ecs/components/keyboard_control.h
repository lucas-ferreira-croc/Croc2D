#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

#include <glm/glm.hpp>

struct KeyBoardControlledComponent
{
	glm::vec2 up_velocity;
	glm::vec2 right_velocity;
	glm::vec2 down_velocity;
	glm::vec2 left_velocity;

	KeyBoardControlledComponent(glm::vec2 up_velocity = glm::vec2(0, 0), glm::vec2 right_velocity = glm::vec2(0, 0), glm::vec2 down_velocity = glm::vec2(0, 0), glm::vec2 left_velocity = glm::vec2(0, 0))
		: up_velocity(up_velocity), right_velocity(right_velocity), down_velocity(down_velocity), left_velocity(left_velocity)
	{

	}
};

#endif