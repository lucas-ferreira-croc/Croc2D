#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

int main(int argc, char* argv[]) {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    glm::vec2 velocity = glm::vec2(5.0, -2.5);
    velocity = glm::normalize(velocity);

    SDL_Init(SDL_INIT_EVERYTHING);

    std::cout << "Hello World!" << std::endl;
    return 0;
}
