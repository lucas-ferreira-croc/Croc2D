#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

struct HealthComponent
{
	int health_percentage;

	HealthComponent(int health_percentage = 0)
		: health_percentage(health_percentage)
	{
	}
};
#endif // !HEALTHCOMPONENT_H

