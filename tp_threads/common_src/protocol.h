#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <utility>
#include <vector>

#include "constants.h"
#include "socket.h"

class Protocol {
private:
    Socket skt;

    std::string create_text_message(const std::string& msg);
    void send_message(const std::string& buf);
    std::string receive_text_message();

    // ======================= SERVER =======================

    std::string board_to_string(const TatetiBoard& g_b);

public:
    // ======================= CLIENT =======================

    Protocol(const std::string& host, const std::string& port);

    void request_create_game(const std::string&);
    void request_join_game(const std::string& name);
    bool await_operation_status();
    void request_list_games();
    std::string await_game_list();
    std::pair<bool, std::string> await_game_update();
    void request_game_move(const PlayerMove& p_move);

    // ======================= SERVER =======================

    explicit Protocol(Socket&& s);

    Operation await_operation();
    void send_operation_confirmed();
    void send_operation_rejected();
    void send_game_list(const std::vector<std::string>& g_list);
    void send_game_update(const TatetiBoard& g_b);
    PlayerMove await_game_move();
    void send_game_result(const TatetiBoard& g_b, const GameResult res);
    void destroy_socket();


    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;

    ~Protocol() = default;
};

#endif
