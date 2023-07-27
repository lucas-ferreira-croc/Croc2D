#pragma once

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H
#include "../ecs.h"

struct SpriteComponent
{
	int width;
	int height;

	SpriteComponent(int width = 0, int height = 0) : width(width), height(height) {}
};
#endif // !SPRITE_COMPONENT_h
