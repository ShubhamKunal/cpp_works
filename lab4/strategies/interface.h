#pragma once
#include "../game/structures.h"
#include <iostream>
#include <set>
class strategy_interface_t {
public:
    virtual step_t commit_first_step(const field_t &field) = 0;

    virtual step_t commit_next_step(const field_t &field, std::pair<size_t, size_t> current_checker) = 0;

    virtual bool check_attack(const field_t &field, std::set<std::pair<int, int>> &attack_checker) = 0;

    virtual bool checkers_attack(const field_t &field, std::set<std::pair<int, int>> &attacked_checkers,
                                   std::pair<int, int> current_checker);

    virtual void if_incorrect_step(const step_t &step) const = 0;

    virtual void if_lose() = 0;

    virtual void if_win() = 0;

    virtual void if_tie() = 0;

    size_t checker_amount = 0;
    size_t player_num = 0;
};