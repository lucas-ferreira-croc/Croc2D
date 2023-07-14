#include "game.h"

#include <iostream>


Game::Game(bool fullscreen, int width, int height)
    : is_running(false), is_full_screen(fullscreen), window_width(width), window_height(height)
{
}

Game::~Game()
{
}

void Game::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }


    window = SDL_CreateWindow(
        "Croc2D", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height, 
        0 
        );

    if(!window)
    {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    is_running = true;


    if (is_full_screen)
    {
        SDL_SetWindowFullscreen(window, is_full_screen);
    }
}

void Game::run()
{
    while (is_running)
    {
        process_input();
        update();
        render();
    }
}

void Game::process_input()
{
    SDL_Event sdl_event;
    while(SDL_PollEvent(&sdl_event))
    {
        switch(sdl_event.type)
        {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                    is_running = false;
                break;
        }
    }
}

void Game::update()
{
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 125, 0, 125, 1);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
