#pragma once
#include <string>
#include "../game/structures.h"
#include "computer_interface.h"

class computer_strategy_t_2 : public computer_interface_t {
public:
    explicit computer_strategy_t_2(std::string name);

    bool check_attack(const field_t &field, std::set<std::pair<int, int>> &attack_checker) override;

    step_t commit_first_step(const field_t &field) override;

    step_t commit_next_step(const field_t &field, std::pair<size_t, size_t> current_checker) override;

    void print_stat() const override;

private:
    std::string name;
};