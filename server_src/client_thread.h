#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <string>
// #include <vector>

#include "../common_src/thread.h"
#include "../common_src/protocol.h"
#include "game_map.h"
#include "../common_src/socket.h"
#include "../common_src/constants.h"

#include "tateti.h"

class ClientThread : public Thread {
    private:
        Protocol protocol;
        GameMap& game_map;
        // Tateti& game; // cambio por variable local
        bool in_game;
        std::string game_name;
        char symbol;
        // std::string game_board; // const TatetiBoard& no se puede creo, voy a tener que hacer .get_board() directo en los parametros de las funciones
        // bool joined_game // bool ovrr_perm (en force_kill(force_stop?) y metodos tateti)


        void handle_create_game(const std::string& g_name) {
            if (game_map.create_game(g_name)) {
                game_name = g_name;
                symbol = P1_SYM;
                in_game = true;
                protocol.send_operation_confirmed();
            } else {
                protocol.send_operation_rejected();
            }
        }

        void handle_join_game(const std::string& g_name) {
            if (game_map.join_game(g_name)) {
                game_name = g_name;
                symbol = P2_SYM;
                in_game = true;
                protocol.send_operation_confirmed();
            } else {
                protocol.send_operation_rejected();
            }
        }


    public:
        ClientThread(Socket&& s, GameMap& gm) : protocol(std::move(s)), game_map(gm), in_game(false) {}

        void run() override {
            try {
                while (not in_game) { // in lobby
                    const Operation op = protocol.await_operation();
                    if (op.type == CREATE_GAME_OP)
                        handle_create_game(op.game_name);
                    else if (op.type == JOIN_GAME_OP)
                        handle_join_game(op.game_name);
                    else if (op.type == LIST_GAMES_OP)
                        protocol.send_game_list(game_map.list_games());
                }

                Tateti& game = game_map.get_game(game_name);

                while (in_game) {
                    if (game.is_over(symbol)) {
                        game.finish_turn(symbol);
                        break;
                    }
                    while (true) {
                        protocol.send_game_update(game.get_board());
                        const PlayerMove p_move = protocol.await_game_move();
                        if (game.make_move(symbol, p_move))
                            break;
                    }
                    game.finish_turn(symbol);
                }

                const char w = game.get_winner();
                if (w == symbol)
                    protocol.send_game_result(game.get_board(), WIN);
                else if (w == NULL_SYM)
                    protocol.send_game_result(game.get_board(), TIE);
                else
                    protocol.send_game_result(game.get_board(), LOSS);

                game_map.leave_game(game_name);
            
            } catch (const std::runtime_error& err) {
                if (in_game) {
                    Tateti& game = game_map.get_game(game_name);
                    game.finish_turn(symbol);
                    game_map.leave_game(game_name);
                }
            }
        }


            // while not in game:
            //     await (create, list o join) // sale de while con game_name y symbol seteados // try {recv msg} catch {return} // este catch es para runtime error que deberia tirar desde protocolo si socket cierra

            // Tateti& game = game_map.get_game()
        
            // while true:
            //     if game.is_over
            //         finish_turn
            //         break
            //     game_board = game.get_board() // esto creo que no va mas
            //     while true
            //         send game update(game_board)
            //         if servidor cierra skt // try {send, await} catch {finish, leave, return} en vez de 2 veces // mismo razon para catch que arriba
            //             finish_turn
            //             game_map.leave_game(hay que ver que paso aca, agrego attr game_name?)
            //             return?
            //         await player move
            //         if servidor cierra skt
            //             finish_turn
            //             game_map.leave_game(hay que ver que paso aca, agrego attr game_name?)
            //             return?
            //         if game.make_move
            //             break
            //     finish_turn
            
            // game.get_winner
            // if w == symbol
            //     send winner msg(game_board) // try-catches
            // else if w == NULL_SYM
            //     send tie msg(game_board)
            // else
            //     send loser msg(game_board)
            // game_map.leave_game(hay que ver que paso aca, agrego attr game_name?)


        void force_stop() {
            protocol.destroy_socket();
            // if joined_game: // si es false es seguro que no jugo el 1er turno
            //     Tateti& game = game_map.get_game()
            //     game.finish_turn((symbol == P1_SYM) ? P2_SYM : P1_SYM) // finish turn del contrario // creo que con esto abarco todas las posibilidades
        }

        ~ClientThread() {}

};

#endif
