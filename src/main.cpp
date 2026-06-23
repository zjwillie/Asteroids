#include "pch.h"

#include <memory>

#include "core/Game.hpp"

//*******************************************
//*******************************************
//******** MAIN         *************
//*******************************************
//*******************************************
int main() {    
    auto game = std::make_unique<Game>();

    game->initialize();
    game->run();
    game->shutdown();

    return 0;
}