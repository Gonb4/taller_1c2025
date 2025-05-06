#include <iostream>
#include <string>
#include <tuple>

#include "../common_src/protocol.h"
#include "../common_src/stdio_manager.h"
#include "../common_src/constants.h"

int main(int argc, const char* argv[]) {
    try {

        if (argc != 3) {
            std::cerr << "Bad program call. Expected: "
                      << argv[0] << " <host> <port>\n";
            return EXIT_FAILURE;
        }

        const std::string host = argv[1];
        const std::string port = argv[2];
        
        Protocol protocol(host, port);
        StdIOManager stdio_mngr;
        std::string game_status;
        bool in_game = false;

        while (not in_game) {
            const Operation op = stdio_mngr.read_operation();
            if (op.type == CREATE_GAME_OP) {
                protocol.request_create_game(op.game_name);
                in_game = protocol.await_operation_status();
            } else if (op.type == JOIN_GAME_OP) {
                protocol.request_join_game(op.game_name);
                in_game = protocol.await_operation_status();
            } else if (op.type == LIST_GAMES_OP) {
                protocol.request_list_games();
                stdio_mngr.print(protocol.await_game_list());
            }
        }

        while (in_game) {
            std::tie(in_game, game_status) = protocol.await_game_update();
            stdio_mngr.print(game_status);
            if (not in_game)
                break;
            const PlayerMove p_move = stdio_mngr.read_game_move();
            protocol.request_game_move(p_move);
        }

        return EXIT_SUCCESS;

    } catch (const std::exception& err) {
        std::cerr << "Exception: " << err.what() << "\n";
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
    }

    return EXIT_FAILURE;
}



// std::string game_status; // simetrico al game_board de ClientThread
// bool in_game?


// while true: (o not in_game)
//     stdio mngr lee const Operation
//     if create
//         protocolo.request_create_game()
//         std::tie - protocolo.await_game_update() // no puedo hacer este wait porque el server no manda nada si create invalid (nombre ya existente)
//     if join
//         protocolo.request_join_game()
//         std::tie - protocolo.await_game_update() // no puedo hacer este wait porque el server no manda nada si join invalid (partida llena o inexistente)
//     if list
//         protocolo.request_list_games()
//         protocolo.await_game_list()


// while true: (o in_game)
//     std::tie - protocolo.await_game_update() // esto deberia ir 1ro si no puedo hacer el await_game_update arriba
//     if not in game:
//         break
//     stdio mngr lee const PlayerMove
//     protocolo.request_game_move()
