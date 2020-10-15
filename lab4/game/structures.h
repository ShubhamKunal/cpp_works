#pragma once

#include <algorithm>

struct step_t {
    step_t(int x1, int y1, int x2, int y2) :
            x1{x1},
            y1{y1},
            x2{x2},
            y2{y2}{}

    int x1, y1, x2, y2;
};

struct apply_step_t {
    apply_step_t(bool correct, size_t player_num):
            correct{correct},
            player_num{player_num}{}

    bool correct;
    size_t player_num;
};

struct field_t {
    field_t() {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((row < 3) && (row + col) % 2 == 1) {
                    fld[row][col] = 'w';
                } else if ((row >= 5) && ((row + col) % 2 == 1)) {
                    fld[row][col] = 'b';
                } else if ((row + col) % 2 == 1) {
                    fld[row][col] = '*';
                } else {
                    fld[row][col] = '.';
                }
            }
        }
    }

    char fld[8][8];
};