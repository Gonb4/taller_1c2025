#include "tateti.h"

Tateti::Tateti(): auth_sym(P1_SYM), _is_over(false), winner_sym(NULL_SYM), turns(0) {
    for (auto& row: board) {
        row.fill(NULL_SYM);
    }

    win_vars[ROWS] = {0, 0, 0};
    win_vars[COLS] = {0, 0, 0};
    win_vars[DIAGS] = {0, 0};

    sym_info[P1_SYM] = {{VAL, P1_VAL}, {WIN_VAL, P1_WIN_VAL}};
    sym_info[P2_SYM] = {{VAL, P2_VAL}, {WIN_VAL, P2_WIN_VAL}};
}


void Tateti::cv_other_turn_is_over(std::unique_lock<std::mutex>& lck, const char p_sym) {
    while (p_sym != auth_sym) {
        other_turn_is_over.wait(lck);
    }
}


bool Tateti::is_over(const char p_sym) {
    std::unique_lock<std::mutex> lck(mtx);
    cv_other_turn_is_over(lck, p_sym);

    return _is_over;
}

bool Tateti::make_move(const char p_sym, const PlayerMove& p_move) {
    std::unique_lock<std::mutex> lck(mtx);
    cv_other_turn_is_over(lck, p_sym);

    if (not is_move_valid(p_move))
        return false;
    board[p_move.row][p_move.col] = p_sym;
    check_winner(p_move);
    if (++turns == MAX_TURNS)
        _is_over = true;

    return true;
}

bool Tateti::is_move_valid(const PlayerMove& p_move) {
    if (p_move.row < 0 || p_move.row > N_ROWS - 1)
        return false;
    if (p_move.col < 0 || p_move.col > N_COLS - 1)
        return false;
    if (board[p_move.row][p_move.col] != NULL_SYM)
        return false;

    return true;
}

void Tateti::check_winner(const PlayerMove& p_move) {
    int n;
    bool game_over = false;

    n = win_vars[ROWS][p_move.row] += sym_info[auth_sym][VAL];
    if (n == sym_info[auth_sym][WIN_VAL])
        game_over = true;

    n = win_vars[COLS][p_move.col] += sym_info[auth_sym][VAL];
    if (n == sym_info[auth_sym][WIN_VAL])
        game_over = true;

    if (p_move.row == p_move.col) {
        n = win_vars[DIAGS][0] += sym_info[auth_sym][VAL];
        if (n == sym_info[auth_sym][WIN_VAL])
            game_over = true;
    }

    if (p_move.row + p_move.col == 2) {
        n = win_vars[DIAGS][1] += sym_info[auth_sym][VAL];
        if (n == sym_info[auth_sym][WIN_VAL])
            game_over = true;
    }

    if (game_over) {
        _is_over = true;
        winner_sym = auth_sym;
    }
}

void Tateti::finish_turn(const char p_sym) {
    std::unique_lock<std::mutex> lck(mtx);
    cv_other_turn_is_over(lck, p_sym);

    auth_sym = (auth_sym == P1_SYM) ? P2_SYM : P1_SYM;
    other_turn_is_over.notify_all();
}

// sin condition variable (turno)
const TatetiBoard& Tateti::get_board() {
    std::unique_lock<std::mutex> lck(mtx);

    return board;
}

char Tateti::get_winner() {
    std::unique_lock<std::mutex> lck(mtx);

    return winner_sym;
}
