#ifndef CONSTANTS_H
#define CONSTANTS_H

inline const char P1_SYM = 'O';
inline const char P2_SYM = 'X';
inline const char NULL_SYM = ' ';

struct PlayerMove {
    const int col;
    const int row;
    
    PlayerMove(const int c, const int r) : col(c), row(r) {}
};

#endif
