
#include "BikeGameSFML.hpp"
#include <iostream>

int main() {
    Game game;

    if (!game.init()) {
        std::cout << "Failed to initialize the game!" << std::endl;
        return -1;
    }

    game.run();

    return 0;
}
