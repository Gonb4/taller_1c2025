#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <cstdint>
#include <string>

#define N_ROWS 3
#define N_COLS 3
#define MAX_TURNS N_ROWS * N_COLS
using TatetiBoard = std::array<std::array<char, N_COLS>, N_ROWS>;
inline const char P1_SYM = 'O';
inline const char P2_SYM = 'X';
inline const char NULL_SYM = ' ';

struct PlayerMove {
    const uint8_t col;
    const uint8_t row;
    
    PlayerMove(const uint8_t c, const uint8_t r) : col(c), row(r) {}
};

enum OperationType {CREATE_GAME_OP, JOIN_GAME_OP, LIST_GAMES_OP};

struct Operation {
    // ListGamesOp
    const OperationType type;
    // CreateGameOp
    // JoinGameOp
    const std::string game_name;

    protected:
        Operation(OperationType t, const std::string& g_n) : type(t), game_name(g_n) {}
};

struct ListGamesOp : public Operation {
    ListGamesOp() : Operation(LIST_GAMES_OP, "") {}
};

struct CreateGameOp : public Operation {
    CreateGameOp(const std::string& g_name) : Operation(CREATE_GAME_OP, g_name) {}
};

struct JoinGameOp : public Operation {
    JoinGameOp(const std::string& g_name) : Operation(JOIN_GAME_OP, g_name) {}
};

enum GameResult {WIN, LOSS, TIE};

#endif
