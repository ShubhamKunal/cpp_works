#pragma once
#include "interface.h"

class human_strategy_t : public strategy_interface_t {
public:
    human_strategy_t(std::string name);

    step_t commit_first_step(const field_t &fld) override;

    void if_incorrect_step(const step_t &step) const override;

    void if_win() override;

    void if_lose() override;

    void if_tie() override;

    step_t commit_next_step(const field_t &fld, std::pair<size_t, size_t> current_checker) override;

    bool check_attack(const field_t &field, std::set<std::pair<int, int>> &attack_checker) override;

private:
    std::string name;
};
