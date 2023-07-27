#include "game.h"
#include "../logger/logger.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/rigidbody_component.h"
#include "../ecs/components/sprite_component.h"

#include "../ecs/systems/movement_system.h"
#include "../ecs/systems/render_system.h"

#include <iostream>

#include <glm/glm.hpp>
#include <SDL_image.h>


Game::Game(bool fullscreen, int width, int height)
    : is_running(false), is_full_screen(fullscreen), window_width(width), window_height(height), registry(std::make_unique<Registry>())
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


void Game::setup()
{
    registry->add_system<MovementSystem>();
    registry->add_system<RenderSystem>();

    Entity tank = registry->create_entity();
    tank.add_component<TransformComponent>(glm::vec2(10.0f, 30.0f), glm::vec2(1.0f, 1.0f), 0.0f);
    tank.add_component<RigidBodyComponent>(glm::vec2(50.0f, 20.0f));
    tank.add_component<SpriteComponent>(10, 10);

    Entity truck = registry->create_entity();
    truck.add_component<TransformComponent>(glm::vec2(50.0f, 10.0f), glm::vec2(1.0f, 1.0f), 0.0f);
    truck.add_component<RigidBodyComponent>(glm::vec2(0.0f, 50.0f));
    truck.add_component<SpriteComponent>(10, 50);
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
    int time_to_wait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - milseconds_previous_frame);
    if(time_to_wait > 0 && time_to_wait <= MILLISECONDS_PER_FRAME)
    {
        SDL_Delay(time_to_wait);
    }
    
    double delta_time = (SDL_GetTicks() - milseconds_previous_frame) / 1000.0f;
    milseconds_previous_frame = SDL_GetTicks();

    
    //MovementSystem.Update();
    //CollisionSystem.Update();
    registry->get_system<MovementSystem>().update(delta_time);

    registry->update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 1);
    SDL_RenderClear(renderer);

    registry->get_system<RenderSystem>().update(renderer);

    SDL_RenderPresent(renderer);
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
