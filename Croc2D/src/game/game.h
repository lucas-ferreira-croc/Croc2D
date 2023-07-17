#ifndef GAME_H
#define GAME_H

#include <SDL.h>

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	int MILISECONDS_PREVIOUS_FRAME = 0;

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