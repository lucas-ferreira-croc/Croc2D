#include "game.h"

#include "../logger/logger.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/rigidbody_component.h"
#include "../ecs/components/sprite_component.h"
#include "../ecs/components/animation_component.h"
#include "../ecs/components/box_collider_component.h"
#include "../ecs/components/keyboard_control.h"
#include "../ecs/components/camera_follow.h"
#include "../ecs/components/projectile_emitter_component.h"
#include "../ecs/components/health_component.h"

#include "../ecs/systems/debug_system.h"
#include "../ecs/systems/movement_system.h"
#include "../ecs/systems/render_system.h"
#include "../ecs/systems/animation_system.h"
#include "../ecs/systems/collision_system.h"
#include "../ecs/systems/damage_system.h"
#include "../ecs/systems/keyboard_movement_system.h"
#include "../ecs/systems/camera_movement_system.h"
#include "../ecs/systems/projectile_emit_system.h"
#include "../ecs/systems/projectile_lifecycle_system.h"

#include <iostream>

#include <glm/glm.hpp>

#include <SDL_image.h>

#include <fstream>

int Game::window_width;
int Game::window_height;
int Game::map_width;
int Game::map_height;

Game::Game(bool fullscreen, int width, int height, bool is_debug)
    : is_running(false), is_full_screen(fullscreen), is_debug(is_debug),
      registry(std::make_unique<Registry>()), asset_store(std::make_unique<AssetStore>()), event_bus(std::make_unique<EventBus>())
{
    window_width = width;
    window_height = height;
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

    camera.x = 0;
    camera.y = 0;
    camera.w = window_width;
    camera.h = window_height;
}

void Game::load_level(int level)
{
    //Add systems

    registry->add_system<MovementSystem>();
    registry->add_system<RenderSystem>();
    registry->add_system<AnimationSystem>();
    registry->add_system<CollissionSystem>();
    registry->add_system<DebugSystem>();
    registry->add_system<DamageSystem>();
    registry->add_system<KeyboardMovementSystem>();
    registry->add_system<CameraMovementSystem>();
    registry->add_system<ProjectileEmitSystem>();
    registry->add_system<ProjectileLifeCycleSystem>();

    //Add assets
    asset_store->add_texture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    asset_store->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    asset_store->add_texture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    asset_store->add_texture(renderer, "radar-image", "./assets/images/radar.png");
    asset_store->add_texture(renderer, "bullet-image", "./assets/images/bullet.png");
    asset_store->add_texture(renderer, "jungle-map", "./assets/tilemaps/jungle.png");


    // load map
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
    map_width = map_num_cols * tile_size * tile_scale;
    map_height = map_num_rows * tile_size * tile_scale; 

    // add entities
    Entity chopper = registry->create_entity();
    chopper.add_component<TransformComponent>(glm::vec2(10.0f, 10.0f), glm::vec2(3.0f, 3.0f), 0.0f);
    chopper.add_component<RigidBodyComponent>(glm::vec2(0.0f, 0.0f));
    chopper.add_component<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.add_component<AnimationComponent>(2, 15, true);
    chopper.add_component<KeyBoardControlledComponent>(glm::vec2(0, -80 * 3), glm::vec2(80 * 3, 0), glm::vec2(0, 80 * 3), glm::vec2(-80 * 3, 0));
    chopper.add_component<CameraFollowComponent>();
    chopper.add_component<HealthComponent>(100);
    chopper.add_component<ProjectileEmitterComponent>(glm::vec2(200 * 3, 200 * 3), 0, 10000, 0, true);


    Entity radar = registry->create_entity();
    radar.add_component<TransformComponent>(glm::vec2(window_width - 140, 10.0f), glm::vec2(2.0f, 2.0f), 0.0f);
    radar.add_component<SpriteComponent>("radar-image", 64, 64, 2, 0, 0, true);
    radar.add_component<AnimationComponent>(8, 5, true);

    Entity tank = registry->create_entity();
    tank.add_component<TransformComponent>(glm::vec2(500.0f, 10.0f), glm::vec2(3.0f, 3.0f), 45.0f);
    tank.add_component<RigidBodyComponent>(glm::vec2(0.1f, 0.1f));
    tank.add_component<SpriteComponent>("tank-image", 32, 32, 2, 0, 0);
    tank.add_component <BoxColliderComponent>(32, 32);
    tank.add_component<ProjectileEmitterComponent>(glm::vec2(100, 0), 5000, 3000, 10, false);
    tank.add_component<HealthComponent>(100);


    Entity truck = registry->create_entity();
    truck.add_component<TransformComponent>(glm::vec2(10.0f, 10.0f), glm::vec2(3.0f, 3.0f), 0.0f);
    truck.add_component<RigidBodyComponent>(glm::vec2(0.1f, 0.1f));
    truck.add_component<SpriteComponent>("truck-image", 32, 32, 1);
    truck.add_component<BoxColliderComponent>(32, 32);
    truck.add_component <ProjectileEmitterComponent>(glm::vec2(100, 100), 2000, 5000, 10, false);
    truck.add_component<HealthComponent>(100);


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
                event_bus->emit_event<KeyPressedEvent>(sdl_event.key.keysym.sym);

                if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                    is_running = false;
                if (sdl_event.key.keysym.sym == SDLK_d)
                    is_debug = !is_debug;
                if (sdl_event.key.keysym.sym == SDLK_SPACE)
                {
                    std::cout << "atirou" << std::endl;
                    event_bus->emit_event<PlayerShootEvent>(registry);
                }
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

    event_bus->reset();

    registry->get_system<DamageSystem>().subscribe_to_events(event_bus);
    registry->get_system<ProjectileEmitSystem>().subscribe_to_event(event_bus);

    registry->get_system<KeyboardMovementSystem>().subscribe_to_events(event_bus);

    registry->update();
 
    registry->get_system<MovementSystem>().update(delta_time);
    registry->get_system<AnimationSystem>().update();
    registry->get_system<CollissionSystem>().update(event_bus);
    registry->get_system<DamageSystem>().update();
    registry->get_system<KeyboardMovementSystem>().update();
    registry->get_system<CameraMovementSystem>().update(camera);
    registry->get_system<ProjectileEmitSystem>().update(registry);
    registry->get_system<ProjectileLifeCycleSystem>().update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 1);
    SDL_RenderClear(renderer);

    registry->get_system<RenderSystem>().update(renderer, asset_store, camera);
    if (is_debug)
    {
        registry->get_system<DebugSystem>().update(renderer, camera);
    }
 

    SDL_RenderPresent(renderer);
}

void Game::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
