#include <iostream>
#include "game/game.h"

int main(int argc, char* argv[]) {
    
    Game game(false);
    game.init();
    game.run();
    game.destroy(); 
    return 0;
}
