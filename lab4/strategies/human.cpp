#include <unordered_set>
#include <unordered_map>
#include "human.h"

human_strategy_t::human_strategy_t(std::string name) :
        name(std::move(name)) {}

void human_strategy_t::if_win() {
    std::cout << name << " - You won!" << std::endl;
}

void human_strategy_t::if_lose() {
    std::cout << name << " - You lost!" << std::endl;
}

void human_strategy_t::if_tie() {
    std::cout << "Tie!" << std::endl;
}

step_t human_strategy_t::commit_first_step(const field_t &fld) {
    std::cout << "Field:" << std::endl;
    std::string cols = "  A  B  C  D  E  F  G  H";
    std::cout << cols << std::endl;
    int i = 1;
    for (const auto &line : fld.fld) {
        std::cout << i << " ";
        i++;
        for (char c : line) {
            std::cout << c << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "Turn of : " << name << std::endl;
    std::cout << "Pick a checker: " << std::endl;

    char x_char;
    int x1 = 42, y1;
    int x2 = 42, y2;
    std::string coords1;
    try {
        std::cin >> coords1;
        x_char = char(coords1[0]);
        y1 = std::stoi(&coords1[1]);
    }catch(const std::exception& obj){
        std::cout << "Russian symbols not supported! \n";
        return {-1, -1, -1, -1};
    }
    std::unordered_map<char, int> mapping = {
              {1, 'A'}
            , {2, 'B'}
            , {3, 'C'}
            , {4, 'D'}
            , {5, 'E'}
            , {6, 'F'}
            , {7, 'G'}
            , {8, 'H'}
    };
    for(auto coord: mapping)
        if(x_char==coord.second){
            x1 = coord.first;
        }

    std::cout << "Commit your step : " << std::endl;
    std::string coords2;
    try {
        std::cin >> coords2;
        x_char = char(coords2[0]);
        y2 = std::stoi(&coords2[1]);
    }catch(const std::exception& obj){
        std::cout << "Russian symbols not supported! \n";
        return {-1, -1, -1, -1};
    }

    for(auto coord: mapping)
        if(x_char==coord.second){
            x2 = coord.first;
        }
    return {x1, y1, x2, y2};
}

step_t human_strategy_t::commit_next_step(const field_t &fld, std::pair<size_t, size_t> current_checker) {
    std::cout << "Field:" << std::endl;
    int i = 1;
    std::string cols = "  A  B  C  D  E  F  G  H";
    std::cout << cols << std::endl;
    for (const auto &line : fld.fld) {
        std::cout << i << " ";
        i++;
        for (char c : line) {
            std::cout << c << "  ";
        }
        std::cout << std::endl;
    }
    int x1 = current_checker.first;
    int y1 = current_checker.second;
    std::cout << "Commit one more step : " << std::endl;
    int x2 = 42, y2;
    char x_char;
    std::string coords;
    try {
        std::cin >> coords;
        x_char = char(coords[0]);
        y2 = std::stoi(&coords[1]);
    }catch(const std::exception& obj){
        std::cout << "Russian symbols not supported!\n";
        return {-1, -1, -1, -1};
    }
    std::unordered_map<char, int> mapping = {
              {1, 'A'}
            , {2, 'B'}
            , {3, 'C'}
            , {4, 'D'}
            , {5, 'E'}
            , {6, 'F'}
            , {7, 'G'}
            , {8, 'H'}
    };
    for(auto coord: mapping)
        if(x_char==coord.second){
            x2 = coord.first;
        }
    return {x1, y1, x2, y2};
}

bool human_strategy_t::check_attack(const field_t &field,
                                    std::set<std::pair<int, int>> &attack_checker) {
    attack_checker.clear();
    bool changed = false;
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            if (player_num == 0) {
                if (field.fld[col][row] == 'w' || field.fld[col][row] == 'W') {
                    if (checkers_attack(field, attack_checker, std::pair<size_t, size_t>(row, col))){ changed = true;}
                }
            } else if(player_num == 1) {
                if (field.fld[col][row] == 'b' || field.fld[col][row] == 'B') {
                    if (checkers_attack(field, attack_checker, std::pair<size_t, size_t>(row, col))){ changed = true;}
                }
            }
        }
    }

    return changed;
}

void human_strategy_t::if_incorrect_step(const step_t &step) const {
    std::cout << "\nIncorrect Step\n" << std::endl;
}