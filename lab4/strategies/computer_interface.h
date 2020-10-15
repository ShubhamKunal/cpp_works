#pragma once
#include "interface.h"

class computer_interface_t : public strategy_interface_t {
public:
    computer_interface_t();

    void if_win() override;

    void if_lose() override;

    void if_tie() override;

    void if_incorrect_step(const step_t &step) const override;

    virtual void print_stat() const;

private:
    size_t win;
    size_t lose;
    size_t tie;
};
