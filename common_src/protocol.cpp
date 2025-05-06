#include "protocol.h"

#include <cstdint>
#include <sstream>
#include <arpa/inet.h>

namespace {
    constexpr int TxtMsgLenBytes = 2;

    constexpr uint8_t CreateGameMsg = 0x6E;
    constexpr uint8_t JoinGameMsg = 0x6A;
    constexpr uint8_t ListGamesMsg = 0x6C;
    constexpr uint8_t GameMoveMsg = 0x70;

    const std::string OperationConfirmed = "OP OK";
    const std::string OperationRejected = "OP FAIL";

    constexpr int GameUpdateLen = 135;

    const std::string WinnerMessage = "Felicitaciones! Ganaste!\n";
    const std::string TieMessage = "La partida ha terminado en empate\n";
    const std::string LoserMessage = "Has perdido. Segui intentando!\n";

    constexpr uint8_t ColMask = 0xF0;
    constexpr uint8_t RowMask = 0x0F;
}


std::string Protocol::create_text_message(const std::string& msg) {
    std::ostringstream oss;

    uint16_t size = htons(msg.size());
    oss.write((const char*)&size, sizeof(size));
    oss << msg;

    return oss.str();
}

void Protocol::send_message(const std::string& buf) {
    if (not skt.sendall(buf.data(), buf.size()))
        throw std::runtime_error("Socket send error: disconnected");
}

std::string Protocol::receive_text_message() {
    uint8_t size_buf[TxtMsgLenBytes];
    if (not skt.recvall(size_buf, TxtMsgLenBytes))
        throw std::runtime_error("Socket receive error: disconnected");

    uint16_t size = ntohs(*(uint16_t*)size_buf);

    char text_buf[size + 1];
    text_buf[size] = '\0';
    if (not skt.recvall(text_buf, size))
        throw std::runtime_error("Socket receive error: disconnected");

    return std::string(text_buf);
}


// ======================= CLIENT =======================

Protocol::Protocol(const std::string& host, const std::string& port) : skt(host.c_str(), port.c_str()) {}


void Protocol::request_create_game(const std::string& name) {
    std::ostringstream oss;

    oss.put(CreateGameMsg);
    oss << create_text_message(name);

    send_message(oss.str());   
}

void Protocol::request_join_game(const std::string& name) {
    std::ostringstream oss;

    oss.put(JoinGameMsg);
    oss << create_text_message(name);

    send_message(oss.str());
}

bool Protocol::await_operation_status() {
    std::string status = receive_text_message();

    if (status == OperationConfirmed)
        return true;
    else if (status == OperationRejected)
        return false;
    else
        throw std::runtime_error("Protocol error: received invalid message");
}

void Protocol::request_list_games() {
    std::ostringstream oss;
    oss.put(ListGamesMsg);
    send_message(oss.str());
}

std::string Protocol::await_game_list() {
    std::string list = receive_text_message();

    return list;
}

std::pair<bool, std::string> Protocol::await_game_update() {
    std::string update = receive_text_message();

    if (update.size() == GameUpdateLen)
        return {true, update};
    else
        return {false, update};
}

void Protocol::request_game_move(const PlayerMove& p_move) {
    std::ostringstream oss;

    oss.put(GameMoveMsg);
    uint8_t coords = (p_move.col << 4) | p_move.row;
    oss.put(coords);

    send_message(oss.str());
}


// ======================= SERVER =======================

Protocol::Protocol(Socket&& s) : skt(std::move(s)) {}


Operation Protocol::await_operation() {
    uint8_t msg_type;
    if (not skt.recvall(&msg_type, 1))
        throw std::runtime_error("Socket receive error: disconnected");

    switch (msg_type) {
        case CreateGameMsg: {
            std::string game_name = receive_text_message();
            return CreateGameOp(game_name);
        }
        case JoinGameMsg: {
            std::string game_name = receive_text_message();
            return JoinGameOp(game_name);
        }
        case ListGamesMsg:
            return ListGamesOp();
        default:
            throw std::runtime_error("Protocol error: received invalid message");
    }
}

void Protocol::send_operation_confirmed() {
    std::string msg = create_text_message(OperationConfirmed);
    send_message(msg);
}

void Protocol::send_operation_rejected() {
    std::string msg = create_text_message(OperationRejected);
    send_message(msg);
}

void Protocol::send_game_list(const std::vector<std::string>& g_list) {
    std::ostringstream oss;
    oss << "Partidas:\n";

    for (const auto& name : g_list) {
        oss << " - " << name << "\n";
    }
    std::string msg = create_text_message(oss.str());

    send_message(msg);
}

void Protocol::send_game_update(const TatetiBoard& g_b) {
    std::string update = board_to_string(g_b);
    std::string msg = create_text_message(update);
    send_message(msg);
}

std::string Protocol::board_to_string(const TatetiBoard& g_b) {
    std::ostringstream oss;

    oss << "    1 . 2 . 3 .\n"
        << "  +---+---+---+\n"
        << "1 | " << g_b[0][0] << " | " << g_b[0][1] << " | " << g_b[0][2] << " |\n"
        << "  +---+---+---+\n"
        << "2 | " << g_b[1][0] << " | " << g_b[1][1] << " | " << g_b[1][2] << " |\n"
        << "  +---+---+---+\n"
        << "3 | " << g_b[2][0] << " | " << g_b[2][1] << " | " << g_b[2][2] << " |\n"
        << "  +---+---+---+\n";

    return oss.str();
}

PlayerMove Protocol::await_game_move() {
    uint8_t msg_type;
    if (not skt.recvall(&msg_type, 1))
        throw std::runtime_error("Socket receive error: disconnected");

    if (msg_type != GameMoveMsg)
        throw std::runtime_error("Protocol error: received invalid message");

    uint8_t coords;
    if (not skt.recvall(&coords, 1))
        throw std::runtime_error("Socket receive error: disconnected");

    uint8_t col = (coords & ColMask) >> 4;
    uint8_t row = (coords & RowMask);
    
    return PlayerMove(col, row);
}

void Protocol::send_game_result(const TatetiBoard& g_b, const GameResult res) {
    std::ostringstream oss;
    oss << board_to_string(g_b);

    switch (res) {
        case WIN:
            oss << WinnerMessage; break;
        case LOSS:
            oss << LoserMessage; break;
        case TIE:
            oss << TieMessage; break;
    }
    std::string msg = create_text_message(oss.str());

    send_message(msg);
}

void Protocol::destroy_socket() {
    skt.shutdown(2);
    skt.close();
}
