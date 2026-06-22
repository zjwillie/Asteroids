#include "pch.h"
#include <memory>
#include <fstream>
#include <string>
#include <iostream>

#include "core/Game.hpp"
#include "utilities/logger/Logger.hpp"
#include "utilities/parser/DronTokenizer.hpp"
#include "utilities/parser/DronParser.hpp" 

//*******************************************
//*******************************************
//******** MAIN         *************
//*******************************************
//*******************************************
int main() {
    Logger::initialize();

    LOG_INFO("Main", "Initializing Game....");
    auto game = std::make_unique<Game>();

    // game->initialize();
    // game->run();
    // game->shutdown();

    DronParser::testEntityRoundTrip("src/utilities/parser/asteroid_test.dron");

    return 0;
}