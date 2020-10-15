#include <memory>

#include "game/game.h"
#include "strategies/human.h"
#include "strategies/computer_first_strategy.h"
#include "strategies/computer_second_strategy.h"

int main() {
    //to play my game >_< use A4, B6  checker input format
    auto player_1 = std::make_shared<computer_strategy_t_2>("Stupid Machine");
    auto player_2 = std::make_shared<human_strategy_t>("Smart Human");
    game_t game(player_1, player_2);
    game.play();
    /*
    auto player_1 = std::make_shared<computer_strategy_t_2>("First player");
    auto player_2 = std::make_shared<computer_strategy_t_1>("Second player");
    for(int i = 0; i<1000; i++){
        game_t game(player1, player2);
        game.play();
    }
    player1->print_stat();
    player2->print_stat();
    */
    return 0;
}