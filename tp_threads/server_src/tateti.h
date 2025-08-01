#ifndef TATETI_H
#define TATETI_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common_src/constants.h"

#define P1_VAL 1
#define P2_VAL -1
#define P1_WIN_VAL 3
#define P2_WIN_VAL -3

enum WinVar { ROWS, COLS, DIAGS };
enum SymData { VAL, WIN_VAL };


class Tateti {
private:
    TatetiBoard board;
    std::unordered_map<WinVar, std::vector<int>> win_vars;
    std::unordered_map<char, std::unordered_map<SymData, int>> sym_info;
    char auth_sym;
    bool _is_over;
    char winner_sym;
    int turns;

    std::mutex mtx;
    std::condition_variable other_turn_is_over;


    void cv_other_turn_is_over(std::unique_lock<std::mutex>& lck, const char p_sym);
    bool is_move_valid(const PlayerMove& p_move);
    void check_winner(const PlayerMove& p_move);

public:
    Tateti();

    bool is_over(const char p_sym);
    bool make_move(const char p_sym, const PlayerMove& p_move);
    void finish_turn(const char p_sym);

    // sin condition variable (turno)
    const TatetiBoard& get_board();
    char get_winner();

    Tateti(const Tateti&) = delete;
    Tateti& operator=(const Tateti&) = delete;

    Tateti(Tateti&&) = default;
    Tateti& operator=(Tateti&&) = default;

    ~Tateti() = default;
};

#endif
