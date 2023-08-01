#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "../ecs/ecs.h"
#include "../asset_store/asset_store.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	int milseconds_previous_frame = 0;

	bool is_running;
	bool is_full_screen;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> asset_store;
public:
	Game(bool fullscreen = true, int width = 1280, int height = 720);
	~Game();

	void init();
	void load_level(int level);
	void setup();
	void run();
	void process_input();
	void update();
	void render();
	void destroy();


	int window_width;
	int window_height;	
};

#endif