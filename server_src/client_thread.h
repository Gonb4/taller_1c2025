#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <vector>

#include "../common_src/thread.h"
#include "../common_src/protocol.h"
#include "tateti.h"

class ClientThread : public Thread {
    private:
        // GameMap&
        Protocol protocol;
        Tateti& game;
        char symbol;
        std::string game_status;

    public:
        void run() override { 
            // while not in game:
            //     await (create, list o join) // sale de while con game y symbol seteados
        
            // while true:
            //     if game.is_over
            //         finish_turn
            //         break
            //     game_status = game.get_status()
            //     send game update(game_status)
            //     await player move
            //     while not game.make_move {}
            //     finish_turn
            
            // game.get_winner
            // if w == symbol
            //     send winner msg(game_status)
            // else
            //     send loser msg(game_status)                        
        }

        ~ClientThread() {}

};

#endif
