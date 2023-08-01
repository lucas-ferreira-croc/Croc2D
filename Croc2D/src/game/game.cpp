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

#include <fstream>
#include <sstream>

Game::Game(bool fullscreen, int width, int height)
    : is_running(false), is_full_screen(fullscreen), window_width(width), window_height(height),
      registry(std::make_unique<Registry>()), asset_store(std::make_unique<AssetStore>())
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

void Game::load_level(int level)
{
    registry->add_system<MovementSystem>();
    registry->add_system<RenderSystem>();

    asset_store->add_texture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    asset_store->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    asset_store->add_texture(renderer, "jungle-map", "./assets/tilemaps/jungle.png");

    int tile_size = 32;
    int tile_scale = 2.0;
    int map_num_cols = 25;
    int map_num_rows = 20;
    
    std::fstream map_file("./assets/tilemaps/jungle.map");
    
    for (int y = 0; y < map_num_rows; y++)
    {
        for(int x = 0; x < map_num_cols; x++)
        {
            char ch;
            map_file.get(ch);
            int src_rect_y = std::atoi(&ch) * tile_size;

            map_file.get(ch);
            int src_rect_x = std::atoi(&ch) * tile_size;
            map_file.ignore();

            Entity tile = registry->create_entity();
            tile.add_component<TransformComponent>(glm::vec2(x * (tile_scale * tile_size), y * (tile_scale * tile_size)), glm::vec2(tile_scale, tile_scale), 0.0f);
            tile.add_component<SpriteComponent>("jungle-map", tile_size, tile_size, 0 ,src_rect_x, src_rect_y);
        }
    }
    map_file.close();
    

    Entity tank = registry->create_entity();
    tank.add_component<TransformComponent>(glm::vec2(10.0f, 10.0f), glm::vec2(3.0f, 3.0f), 45.0f);
    tank.add_component<RigidBodyComponent>(glm::vec2(40.0f, 0.0f));
    tank.add_component<SpriteComponent>("tank-image", 32, 32, 2, 0, 0);

    Entity truck = registry->create_entity();
    truck.add_component<TransformComponent>(glm::vec2(32.0f, 32.0f), glm::vec2(1.0f, 1.0f), 0.0f);
    truck.add_component<RigidBodyComponent>(glm::vec2(50.0f, 00.0f));
    truck.add_component<SpriteComponent>("truck-image", 32, 32, 1);
}


void Game::setup()
{
    load_level(1);
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

    registry->get_system<RenderSystem>().update(renderer, asset_store);

    SDL_RenderPresent(renderer);
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
