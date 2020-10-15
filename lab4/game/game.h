#pragma once
#include <memory>
#include <vector>
#include "../strategies/interface.h"
#include "structures.h"

class game_t {
public:
    using player_t = std::shared_ptr<strategy_interface_t>;

    game_t(const player_t &first, const player_t &second);

    void play();

private:
    enum game_outcome_t {
        WIN,
        TIE,
        IN_PROGRESS
    };

    game_outcome_t winning(size_t player_num) const;
    apply_step_t apply_step(const step_t &step, size_t player_num, std::set<std::pair<int, int> > &attack_checkers);

    field_t field;
    std::vector<player_t> players;
    size_t steps_amount = 0;
};