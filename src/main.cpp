#include <memory>

#include "core/Game.hpp"

#include "utilities/logger/Logger.hpp"
#include "utilities/parser/DronTokenizer.hpp"

//*******************************************
//*******************************************
//********         MAIN         *************
//*******************************************
//*******************************************


int main() {
    auto game = std::make_unique<Game>();

    game->initialize();
    game->run();
    game->shutdown();

    // Testing Tokenizer
    
    //Logger::initialize();

    //DronTokenizer::test("src/utilities/parser/test.dron");

    return 0;
}