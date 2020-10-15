#include "computer_first_strategy.h"
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

computer_strategy_t_1::computer_strategy_t_1(std::string name): name(std::move(name)) {}

step_t computer_strategy_t_1::commit_first_step(const field_t &fld) {
    char letter, capital_letter, opposite_letter, opposite_capital_letter;
    std::set<std::pair<int, int>> kill;
    std::set<std::pair<int, int>> take_step;
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    if (player_num == 0){letter = 'w'; capital_letter = 'W'; opposite_letter = 'b'; opposite_capital_letter = 'B';}
    else{letter = 'b'; capital_letter = 'B'; opposite_letter = 'w'; opposite_capital_letter = 'W';}
    bool changed;
    for (int row_ = 0; row_ < 8; row_++) {
        for (int col_ = 0; col_ < 8; col_++) {
            if (fld.fld[row_][col_] == letter || fld.fld[row_][col_] == capital_letter) {
                changed = checkers_attack(fld, kill, std::pair<size_t, size_t>(col_, row_));

                if (!changed) {
                    if(player_num == 0){
                        if ((fld.fld[row_ + 1][col_ + 1] == '*' || fld.fld[row_ + 1][col_ - 1] == '*') &&
                            fld.fld[row_][col_] == letter) {
                            take_step.insert(std::pair<size_t, size_t>(col_, row_));
                        }
                        if ((fld.fld[row_ + 1][col_ + 1] == '*' || fld.fld[row_ + 1][col_ - 1] == '*' ||
                             fld.fld[row_ - 1][col_ + 1] == '*' || fld.fld[row_ - 1][col_ - 1] == '*') &&
                            fld.fld[row_][col_] == capital_letter) {
                            take_step.insert(std::pair<size_t, size_t>(col_, row_));
                        }
                    } else{
                        if ((fld.fld[row_ - 1][col_ + 1] == '*' || fld.fld[row_ - 1][col_ - 1] == '*') &&
                            fld.fld[row_][col_] == letter) {
                            take_step.insert(std::pair<size_t, size_t>(col_, row_));
                        }
                        if ((fld.fld[row_ + 1][col_ + 1] == '*' || fld.fld[row_ + 1][col_ - 1] == '*' ||
                             fld.fld[row_ - 1][col_ + 1] == '*' || fld.fld[row_ - 1][col_ - 1] == '*') &&
                            fld.fld[row_][col_] == capital_letter) {
                            take_step.insert(std::pair<size_t, size_t>(col_, row_));
                        }
                    }
                }
            }
        }
    }
    std::vector<std::pair<int, int>> checkMoves;
    if (!kill.empty()) {
        std::vector<std::pair<int, int>> possible_checkers(kill.begin(), kill.end());
        std::shuffle(possible_checkers.begin(), possible_checkers.end(), std::mt19937(std::random_device()()));
        selected_checker = possible_checkers.front();
        if (fld.fld[selected_checker.second][selected_checker.first] == capital_letter ||
            fld.fld[selected_checker.second][selected_checker.first] == letter) {
            int length;
            if (fld.fld[selected_checker.second][selected_checker.first] == capital_letter) {
                length = std::max(selected_checker.second, 7 - selected_checker.second);
            } else {length = 2;}
            int change_step[2] = {-1, 1};
            for (auto &i: change_step) {
                for (auto &j: change_step) {
                    bool enemy = false;
                    for (int point = 1; point <= length; ++point) {
                        auto x = selected_checker.second + i * point;
                        auto y = selected_checker.first + j * point;
                        if (enemy && fld.fld[x][y] != '*') {break;}
                        if (fld.fld[x][y] == opposite_letter || fld.fld[x][y] == opposite_capital_letter) {enemy = true;}
                        if (enemy && fld.fld[x][y] == '*') {checkMoves.emplace_back(y,x);}
                    }
                }
            }
        }
        std::shuffle(checkMoves.begin(), checkMoves.end(), std::mt19937(std::random_device()()));
        selected_target = checkMoves.front();
    } else if (!take_step.empty()) {
        std::vector<std::pair<int, int>> possible_checkers(take_step.begin(), take_step.end());
        std::shuffle(possible_checkers.begin(), possible_checkers.end(), std::mt19937(std::random_device()()));
        selected_checker = possible_checkers.front();
        if(player_num == 0){
            if (fld.fld[selected_checker.second][selected_checker.first] == letter) {
                if (fld.fld[selected_checker.second + 1][selected_checker.first + 1] == '*') {
                    checkMoves.emplace_back(selected_checker.first + 1, selected_checker.second + 1);
                } else {
                    checkMoves.emplace_back(selected_checker.first - 1, selected_checker.second + 1);
                }
            }
        } else{
            if (fld.fld[selected_checker.second][selected_checker.first] == letter) {
                if (fld.fld[selected_checker.second - 1][selected_checker.first + 1] == '*') {
                    checkMoves.emplace_back(selected_checker.first + 1, selected_checker.second - 1);
                } else {
                    checkMoves.emplace_back(selected_checker.first - 1, selected_checker.second - 1);
                }
            }
        }
        if (fld.fld[selected_checker.second][selected_checker.first] == capital_letter) {
            size_t length = std::max(selected_checker.second - 0, 7 - selected_checker.second);
            int change_step[2] = {-1, 1};
            for (auto &i: change_step) {
                for (auto &j: change_step) {
                    for (int point = 1; point < length; ++point) {
                        if (fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == '*') {
                            checkMoves.emplace_back(selected_checker.first + j * point,
                                                        selected_checker.second + i * point);
                        } else {break;}
                    }
                }
            }
        }
        selected_target = checkMoves.back();
    }
    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}

bool computer_strategy_t_1::check_attack(const field_t &fld, std::set<std::pair<int, int> > &attack_checker) {
    attack_checker.clear();
    bool changed = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (player_num == 0) {
                if (fld.fld[i][j] == 'w' || fld.fld[i][j] == 'W') {
                    if (checkers_attack(fld, attack_checker, std::pair<size_t, size_t>(j, i))) {
                        changed = true;
                    }
                }
            } else {
                if (fld.fld[i][j] == 'b' || fld.fld[i][j] == 'B') {
                    if (checkers_attack(fld, attack_checker, std::pair<size_t, size_t>(j, i))) {
                        changed = true;
                    }
                }
            }
        }
    }
    return changed;
}

step_t computer_strategy_t_1::commit_next_step(const field_t &fld, std::pair<size_t, size_t> current_checker) {
    char letter, capital_letter, opposite_letter, opposite_capital_letter;
    if (player_num == 0){letter = 'w'; capital_letter = 'W'; opposite_letter = 'b'; opposite_capital_letter = 'B';}
    else{letter = 'b'; capital_letter = 'B'; opposite_letter = 'w'; opposite_capital_letter = 'W';}
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    std::vector<std::pair<int, int>> possible_moves;
    selected_checker.first = current_checker.first - 1;
    selected_checker.second = current_checker.second - 1;
    if (fld.fld[selected_checker.second][selected_checker.first] == letter) {
        int change_step[2] = {-1, 1};
        for (auto &i: change_step) {
            for (auto &j: change_step) {
                if (fld.fld[selected_checker.second + i][selected_checker.first + j] == opposite_letter ||
                    fld.fld[selected_checker.second + i][selected_checker.first + j] == opposite_capital_letter) {
                    if (fld.fld[selected_checker.second + i * 2][selected_checker.first + j * 2] == '*') {
                        possible_moves.emplace_back(selected_checker.first + j * 2,
                                                    selected_checker.second + i * 2);

                    }
                }
            }
        }
    }
    if (fld.fld[selected_checker.second][selected_checker.first] == capital_letter) {
        size_t length = std::max(selected_checker.second, 7 - selected_checker.second);
        int change_step[2] = {-1, 1};
        for (auto &i: change_step) {
            for (auto &j: change_step) {
                bool enemy = false;
                for (int point = 1; point <= length; ++point) {
                    if (enemy &&
                        fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] != '*') {
                        break;
                    }
                    if (fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == opposite_letter ||
                        fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == opposite_capital_letter) {
                        enemy = true;
                    }
                    if (enemy &&
                        fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == '*') {
                        possible_moves.emplace_back(selected_checker.first + j * point,
                                                    selected_checker.second + i * point);
                    }
                }
            }
        }
    }
    selected_target = possible_moves.back();
    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}

void computer_strategy_t_1::print_stat() const {
    std::cout << "Computer " << player_num + 1 << ": " << std::endl;
    computer_interface_t::print_stat();
}