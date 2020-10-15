#include "computer_second_strategy.h"

#include <cassert>
#include <iostream>
#include <random>
#include <vector>


computer_strategy_t_2::computer_strategy_t_2(std::string name) : name(std::move(name)) {}

step_t computer_strategy_t_2::commit_first_step(const field_t &fld) {
    char letter, capitalLetter, oppositeLetter, oppositeCapitalLetter;
    std::set<std::pair<int, int>> kill;
    std::set<std::pair<int, int>> make_step;
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    if (player_num == 0){letter = 'w'; capitalLetter = 'W'; oppositeLetter = 'b'; oppositeCapitalLetter = 'B';}
    else{letter = 'b'; capitalLetter = 'B'; oppositeLetter = 'w'; oppositeCapitalLetter = 'W';}
    bool changed;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (fld.fld[row][col] == letter || fld.fld[row][col] == capitalLetter) {
                changed = checkers_attack(fld, kill, std::pair<size_t, size_t>(col, row));

                if (!changed) {
                    if(player_num == 0){
                        if ((fld.fld[row + 1][col + 1] == '*' || fld.fld[row + 1][col - 1] == '*') &&
                            fld.fld[row][col] == letter) {
                            make_step.insert(std::pair<size_t, size_t>(col, row));
                        }
                        if ((fld.fld[row + 1][col + 1] == '*' || fld.fld[row + 1][col - 1] == '*' ||
                             fld.fld[row - 1][col + 1] == '*' || fld.fld[row - 1][col - 1] == '*') &&
                            fld.fld[row][col] == capitalLetter) {
                            make_step.insert(std::pair<size_t, size_t>(col, row));
                        }
                    } else{
                        if ((fld.fld[row - 1][col + 1] == '*' || fld.fld[row - 1][col - 1] == '*') &&
                            fld.fld[row][col] == letter) {
                            make_step.insert(std::pair<size_t, size_t>(col, row));
                        }
                        if ((fld.fld[row + 1][col + 1] == '*' || fld.fld[row + 1][col - 1] == '*' ||
                             fld.fld[row - 1][col + 1] == '*' || fld.fld[row - 1][col - 1] == '*') &&
                            fld.fld[row][col] == capitalLetter) {
                            make_step.insert(std::pair<size_t, size_t>(col, row));
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
        if (fld.fld[selected_checker.second][selected_checker.first] == capitalLetter ||
            fld.fld[selected_checker.second][selected_checker.first] == letter) {
            int length;
            if (fld.fld[selected_checker.second][selected_checker.first] == capitalLetter) {
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
                        if (fld.fld[x][y] == oppositeLetter || fld.fld[x][y] == oppositeCapitalLetter) {enemy = true;}
                        if (enemy && fld.fld[x][y] == '*') {checkMoves.emplace_back(y,x);}
                    }
                }
            }
        }
        std::shuffle(checkMoves.begin(), checkMoves.end(), std::mt19937(std::random_device()()));
        selected_target = checkMoves.front();
    } else if (!make_step.empty()) {
        std::vector<std::pair<int, int>> possible_checkers(make_step.begin(), make_step.end());
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
        if (fld.fld[selected_checker.second][selected_checker.first] == capitalLetter) {
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
        std::shuffle(checkMoves.begin(), checkMoves.end(), std::mt19937(std::random_device()()));
        selected_target = checkMoves.front();
    }
    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}

bool computer_strategy_t_2::check_attack(const field_t &fld, std::set<std::pair<int, int> > &attack_checker) {
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

step_t computer_strategy_t_2::commit_next_step(const field_t &fld, std::pair<size_t, size_t> current_checker) {
    char letter, capitalLetter, oppositeLetter, oppositeCapitalLetter;
    if (player_num == 0){letter = 'w'; capitalLetter = 'W'; oppositeLetter = 'b'; oppositeCapitalLetter = 'B';}
    else{letter = 'b'; capitalLetter = 'B'; oppositeLetter = 'w'; oppositeCapitalLetter = 'W';}
    std::pair<int, int> selected_checker;
    std::pair<int, int> selected_target;
    std::vector<std::pair<int, int>> possible_moves;
    selected_checker.first = current_checker.first - 1;
    selected_checker.second = current_checker.second - 1;
    if (fld.fld[selected_checker.second][selected_checker.first] == letter) {
        int change_step[2] = {-1, 1};
        for (auto &i: change_step) {
            for (auto &j: change_step) {
                if (fld.fld[selected_checker.second + i][selected_checker.first + j] == oppositeLetter ||
                    fld.fld[selected_checker.second + i][selected_checker.first + j] == oppositeCapitalLetter) {
                    if (fld.fld[selected_checker.second + i * 2][selected_checker.first + j * 2] == '*') {
                        possible_moves.emplace_back(selected_checker.first + j * 2,
                                                    selected_checker.second + i * 2);

                    }
                }
            }
        }
    }
    if (fld.fld[selected_checker.second][selected_checker.first] == capitalLetter) {
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
                    if (fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == oppositeLetter ||
                        fld.fld[selected_checker.second + i * point][selected_checker.first + j * point] == oppositeCapitalLetter) {
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
    std::shuffle(possible_moves.begin(), possible_moves.end(), std::mt19937(std::random_device()()));
    selected_target = possible_moves.front();
    return {selected_checker.first + 1, selected_checker.second + 1, selected_target.first + 1,
            selected_target.second + 1};
}
void computer_strategy_t_2::print_stat() const {
    std::cout << "Computer " << player_num + 1 << ": " << std::endl;
    computer_interface_t::print_stat();
}