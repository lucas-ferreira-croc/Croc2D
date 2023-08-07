#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ecs/ecs.h"
#include "../asset_store/asset_store.h"
#include "../event_bus/event_bus.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	int milseconds_previous_frame = 0;

	bool is_running;
	bool is_debug;
	bool is_full_screen;

	SDL_Rect camera;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> asset_store;
	std::unique_ptr<EventBus> event_bus;
public:
	Game(bool fullscreen = true, int width = 1280, int height = 720, bool is_debug = false);
	~Game();

	void init();
	void load_level(int level);
	void setup();
	void run();
	void process_input();
	void update();
	void render();
	void destroy();


	static int window_width;
	static int window_height;	

	static int map_width;
	static int map_height;
};

#endif