#include "computer_interface.h"
#include <iostream>

computer_interface_t::computer_interface_t() :
        win{0},
        lose{0},
        tie{0} {}

void computer_interface_t::if_win() {
    win += 1;
}

void computer_interface_t::if_lose() {
    lose += 1;
}

void computer_interface_t::if_tie() {
    tie += 1;
}

void computer_interface_t::if_incorrect_step(const step_t &step) const {
    std::cout << "Computer got confused!";
    exit(-1);
}

void computer_interface_t::print_stat() const {
    std::cout << "Count of wins:  " << win << std::endl;
    std::cout << "Count of loses: " << lose << std::endl;
    std::cout << "Count of ties:  " << tie << std::endl;
    std::cout << std::endl;
}
