#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <glm/glm.hpp>

struct BoxColliderComponent
{
	int width;
	int height;
	bool is_colliding;

	glm::vec2 offset;

	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0.0f), bool is_colliding = false)
		: width(width), height(height), offset(offset), is_colliding(is_colliding)
	{
	}
};

#endif
