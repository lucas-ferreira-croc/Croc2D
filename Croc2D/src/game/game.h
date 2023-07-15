#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	bool is_running;
	bool is_full_screen;
public:
	Game(bool fullscreen = true, int width = 1280, int height = 720);
	~Game();

	void init();
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