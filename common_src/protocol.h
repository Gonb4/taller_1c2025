#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "constants.h"
#include "socket.h"

class Protocol {
    private:
        Socket skt;

        // client
        // void request_create_game(const Operation& op); // o const std::string&
        // void request_list_games();
        // void request_join_game(const Operation& op); // o const std::string&

        // server
        // Operation await_create_game();
        // Operation await_list_games();
        // Operation await_join_game();

    public:
        // client
        Protocol(const std::string& hostname, const std::string& servname);

        // void request_operation(const Operation& op); // return value para list_games?
        std::string await_game_update();
        void request_game_move(const PlayerMove& p_move);


        // server
        explicit Protocol(Socket&& s);

        // Operation await_operation();
        void send_game_update(const std::string& g_status);
        PlayerMove await_player_move();

        Protocol(const Protocol&) = delete;
        Protocol& operator=(const Protocol&) = delete;

        Protocol(Protocol&&) = default;
        Protocol& operator=(Protocol&&) = default;

        ~Protocol() = default;
};

#endif
