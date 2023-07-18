#include "game.h"
#include "../logger/logger.h"
#include <iostream>

#include <glm/glm.hpp>
#include <SDL_image.h>

Game::Game(bool fullscreen, int width, int height)
    : is_running(false), is_full_screen(fullscreen), window_width(width), window_height(height)
{
    Logger::log("Game constructor called");
}

Game::~Game()
{
    Logger::log("Game destructor called");
}

void Game::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        Logger::error("Error initializing SDL");
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
        Logger::error("Error creating SDL window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        Logger::error("Error creating SDL renderer");
        return;
    }

    is_running = true;


    if (is_full_screen)
    {
        SDL_SetWindowFullscreen(window, is_full_screen);
    }
}

glm::vec2 player_pos;
glm::vec2 player_velocity;

void Game::setup()
{
    player_pos = glm::vec2(10.0f, 20.0f);

    player_velocity = glm::vec2(100.0f, 50.0f);
}

void Game::run()
{
    setup();
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
    // Delay and frame cap
    int time_to_wait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - MILISECONDS_PREVIOUS_FRAME);
    if(time_to_wait > 0 && time_to_wait <= MILLISECONDS_PER_FRAME)
    {
        SDL_Delay(time_to_wait);
    }
    
    double delta_time = (SDL_GetTicks() - MILISECONDS_PREVIOUS_FRAME) / 1000.0f;
    MILISECONDS_PREVIOUS_FRAME = SDL_GetTicks();

    player_pos.x += player_velocity.x * delta_time;
    player_pos.y += player_velocity.y * delta_time;
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 1);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect destination_rect = { 
        static_cast<int>(player_pos.x), 
        static_cast<int>(player_pos.y),
        128, 
        128 
    };

    SDL_RenderCopy(renderer, texture, NULL, &destination_rect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
