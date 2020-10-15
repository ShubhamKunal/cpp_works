#include <iostream>
#include <set>
#include "game.h"
#include "../strategies/interface.h"

char letter = 'P', capital_letter = 'A', opposite_letter = 'I', opposite_capital_letter = 'N';

game_t::game_t(const player_t &first, const player_t &second) : field() {
    players.push_back(first);
    players[0]->player_num = 0;
    players[0]->checker_amount = 12;
    players.push_back(second);
    players[1]->player_num = 1;
    players[1]->checker_amount = 12;
}

bool strategy_interface_t::checkers_attack(const field_t &field, std::set<std::pair<int, int> > &attack_checkers,
                                             std::pair<int, int> current_checker) {
    bool transformed = false;
    int transformed_step[2] = {-1, 1};
    auto field_ = field.fld[current_checker.second][current_checker.first];
    if (field_ == 'w' || field_ == 'W') {
        letter = 'w';
        capital_letter = 'W';
        opposite_letter = 'b';
        opposite_capital_letter = 'B';
    } else {
        letter = 'b';
        capital_letter = 'B';
        opposite_letter = 'w';
        opposite_capital_letter = 'W';
    }

    if (field_ == letter) {
        for (auto &col_: transformed_step) {
            for (auto &row_: transformed_step) {
                int x = current_checker.second + col_;
                int y = current_checker.first + row_;
                int x_ = current_checker.second + col_ * 2;
                int y_ = current_checker.first + row_ * 2;
                if (field.fld[x][y] == opposite_letter || field.fld[x][y] == opposite_capital_letter) {
                    if (field.fld[x_][y_] == '*') {
                        attack_checkers.insert(
                                std::pair<size_t, size_t>(current_checker.first, current_checker.second));
                        transformed = true;
                    }
                }
            }
        }
    } else if (field_ == capital_letter) {
        size_t max_cell = std::max(current_checker.second, 7 - current_checker.second);
        for (auto &col_: transformed_step) {
            for (auto &row_: transformed_step) {
                for (int cell = 1; cell < max_cell; ++cell) {
                    int x = current_checker.second + col_ * cell;
                    int y = current_checker.first + row_ * cell;
                    int x_ = current_checker.second + col_ * (cell + 1);
                    int y_ = current_checker.first + row_ * (cell + 1);
                    if (field.fld[x][y] == opposite_letter || field.fld[x][y] == opposite_capital_letter) {
                        if (field.fld[x_][y_] != '*') { break; }
                        else {
                            attack_checkers.insert(
                                    std::pair<size_t, size_t>(current_checker.first, current_checker.second));
                            transformed = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    return transformed;
}

apply_step_t
game_t::apply_step(const step_t &step, size_t player_num, std::set<std::pair<int, int> > &attack_checkers) {
    char &x1y1 = field.fld[step.y1 - 1][step.x1 - 1];
    char &x2y2 = field.fld[step.y2 - 1][step.x2 - 1];
    bool kill = players[player_num]->check_attack(field, attack_checkers);
    size_t next_player = std::abs(int(player_num - 1));
    if ((x2y2 != '*') || (x1y1 == '.') || (x1y1 == '*')) {
        return {false, player_num};
    }

    if (player_num == 0) {
        letter = 'w';
        capital_letter = 'W';
        opposite_letter = 'b';
        opposite_capital_letter = 'B';
    } else {
        letter = 'b';
        capital_letter = 'B';
        opposite_letter = 'w';
        opposite_capital_letter = 'W';
    }

    int next_step_;
    player_num == 0 ? next_step_ = 8 : next_step_ = 1;

    if (x1y1 == letter && !kill && (((step.y2 - step.y1) == 1 && player_num == 0) ||
        ((step.y1 - step.y2) == 1 && player_num == 1)) && (abs(step.x2 - step.x1) ||
        abs(step.x1 - step.x2)) == 1) {
        //if we just walk
        std::swap(x2y2, x1y1);
        return {true, next_player};


    } else if (x1y1 == letter && !kill && (((step.y2 - step.y1) == 1 && player_num == 0)||
              ((step.y1 - step.y2) == 1 && player_num == 1)) && (abs(step.x2 - step.x1) ||
              abs(step.x1 - step.x2)) == 1 && step.y2 == next_step_) {
        // become a queen
        std::swap(x2y2, x1y1);
        x2y2 = capital_letter;
        return {true, next_player};

    } else if (x1y1 == capital_letter && !kill
               && abs(step.x2 - step.x1) == abs(step.y2 - step.y1)) {
        //just walk the king
        bool nobody2_kill = true;
        for (int i = 1; i < abs(step.y2 - step.y1); ++i) {
            if (field.fld[step.y1 + i * (step.y2 - step.y1) /
                                    abs(step.y2 - step.y1) - 1][step.x1 + i * (step.x2 - step.x1) /
                                                                          abs(step.x2 - step.x1) - 1] != '*') {
                //no pawns
                nobody2_kill = false;
            }
        }
        if (nobody2_kill) {
            std::swap(x2y2, x1y1);
            return {true, next_player};
        }
    } else if (kill && attack_checkers.count(std::pair<size_t, size_t>(step.x1 - 1, step.y1 - 1))) {
        bool next_attack;
        step_t next_step = step;
        do {
            bool correct_move = false;
            while (!correct_move) {
                if (abs(next_step.y1 - next_step.y2) == 2 &&
                    abs(next_step.x1 - next_step.x2) == 2 &&
                    field.fld[next_step.y2 - 1][next_step.x2 - 1] == '*') {
                    size_t min_y = std::min(next_step.y2, next_step.y1);
                    size_t min_x = std::min(next_step.x2, next_step.x1);
                    char &middle_cell = field.fld[min_y][min_x];
                    if (middle_cell == opposite_letter || middle_cell == opposite_capital_letter) {
                        --players[next_player]->checker_amount;
                        middle_cell = '*';
                        std::swap(field.fld[next_step.y1 - 1][next_step.x1 - 1],
                                  field.fld[next_step.y2 - 1][next_step.x2 - 1]);

                        if (next_step.y2 == next_step_) {
                            field.fld[next_step.y2 - 1][next_step.x2 - 1] = capital_letter;
                            return {true, next_player};
                        }
                        correct_move = true;
                    }
                } else if (abs(next_step.y1 - next_step.y2) == abs(next_step.x1 - next_step.x2) &&
                           abs(next_step.x1 - next_step.x2) > 2 &&
                           field.fld[next_step.y2 - 1][next_step.x2 - 1] == '*') {
                    int coefX = (next_step.x2 - next_step.x1) / abs(next_step.x2 - next_step.x1);
                    int coefY = (next_step.y2 - next_step.y1) / abs(next_step.y2 - next_step.y1);
                    bool enemy = false;
                    std::pair<size_t, size_t> enemy_pos;
                    for (int i = 1; i < abs(next_step.y2 - next_step.y1); ++i) {
                        if (enemy &&
                            field.fld[next_step.y1 + i * coefY - 1][next_step.x1 + i * coefX - 1] != '*') {
                            enemy = false;
                            break;
                        }
                        if (field.fld[next_step.y1 + i * coefY - 1][next_step.x1 + i * coefX - 1] == opposite_letter ||
                            field.fld[next_step.y1 + i * coefY - 1][next_step.x1 + i * coefX - 1] ==
                            opposite_capital_letter) {
                            enemy = true;
                            enemy_pos.first = next_step.x1 + i * coefX - 1;
                            enemy_pos.second = next_step.y1 + i * coefY - 1;
                        }
                    }
                    if (enemy) {
                        --players[next_player]->checker_amount;
                        std::swap(field.fld[next_step.y1 - 1][next_step.x1 - 1],
                                  field.fld[next_step.y2 - 1][next_step.x2 - 1]);
                        field.fld[enemy_pos.second][enemy_pos.first] = '*';
                        if (next_step.y2 == next_step_ &&
                            field.fld[next_step.y2 - 1][next_step.x2 - 1] != capital_letter) {
                            field.fld[next_step.y2 - 1][next_step.x2 - 1] = capital_letter;
                            return {true, next_player};
                        }
                        correct_move = true;
                    }
                }
                if (!correct_move) {
                    players[player_num]->if_incorrect_step(next_step);
                    next_step = players[player_num]->commit_next_step(field,
                                                                    std::pair<size_t, size_t>(next_step.x1,
                                                                                              next_step.y1));
                }
            }
            next_attack = players[player_num]->checkers_attack(field,
                                                                 attack_checkers,
                                                                 std::pair<size_t, size_t>(next_step.x2 - 1,
                                                                                           next_step.y2 - 1));
            if (next_attack) {
                next_step = players[player_num]->commit_next_step(field,
                                                                std::pair<size_t, size_t>(next_step.x2, next_step.y2));
            }
        } while (next_attack);
        return {true, next_player};
    }
    return {false, player_num};
}

void game_t::play() {
    std::set<std::pair<int, int>> attack_checkers;
    apply_step_t apply_result(false, 0);
    size_t counter = apply_result.player_num;
    while (winning(counter) == IN_PROGRESS) {
        ++steps_amount;
        counter = apply_result.player_num;
        bool is_correct = false;
        while (!is_correct) {
            step_t step = players[counter]->commit_first_step(field);
            apply_result = apply_step(step, counter, attack_checkers);
            is_correct = apply_result.correct;
            if (!is_correct) {
                players[counter]->if_incorrect_step(step);
            }
        }
    }

    if (winning(counter) == TIE) {
        for (const auto &p : players) {
            p->if_tie();
        }
        return;
    }

    for (size_t i = 0; i < players.size(); ++i) {
        if (i == counter) {
            players[i]->if_win();
        } else {
            players[i]->if_lose();
        }
    }
}

game_t::game_outcome_t game_t::winning(size_t player_num) const {
    std::set<std::pair<int, int>> attack_checkers;
    player_num = std::abs(int(player_num - 1));
    bool can_move = false;
    for (int row_ = 0; row_ < 8; row_++) {
        for (int col_ = 0; col_ < 8; col_++) {
            if (player_num == 0) {
                if (field.fld[row_][col_] == 'w' &&
                    (field.fld[row_ + 1][col_ + 1] == '*' || field.fld[row_ + 1][col_ - 1] == '*')) {
                    can_move = true;
                }
                if (field.fld[row_][col_] == 'w' || field.fld[row_][col_] == 'W') {
                    int transformed_step[2] = {-1, 1};
                    for (auto &i: transformed_step) {
                        for (auto &j: transformed_step) {
                            if (field.fld[row_ + i][col_ + j] == '*' && field.fld[row_][col_] == 'W') {
                                can_move = true;
                                break;
                            }
                            if ((field.fld[row_ + i][col_ + j] == 'b' || field.fld[row_ + i][col_ + j] == 'B') &&
                                field.fld[row_ + i * 2][col_ + j * 2] == '*') {
                                can_move = true;
                                break;
                            }
                        }
                    }
                }
            } else {
                if (field.fld[row_][col_] == 'b' &&
                    (field.fld[row_ - 1][col_ + 1] == '*' || field.fld[row_ - 1][col_ - 1] == '*')) {
                    can_move = true;
                }
                if (field.fld[row_][col_] == 'b' || field.fld[row_][col_] == 'B') {
                    int transformed_step[2] = {-1, 1};
                    for (auto &i: transformed_step) {
                        for (auto &j: transformed_step) {
                            if (field.fld[row_ + i][col_ + j] == '*' && field.fld[row_][col_] == 'B') {
                                can_move = true;
                                break;
                            }
                            if ((field.fld[row_ + i][col_ + j] == 'w' || field.fld[row_ + i][col_ + j] == 'W') &&
                                field.fld[row_ + i * 2][col_ + j * 2] == '*') {
                                can_move = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!can_move) {
        return WIN;
    }

    if (steps_amount > 1000) {
        return TIE;
    }

    if (players[0]->checker_amount == 0 || players[1]->checker_amount == 0) {
        return WIN;

    }
    return IN_PROGRESS;
}