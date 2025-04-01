#include "bin_protocol.h"

#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "constants.h"


BinaryProtocol::BinaryProtocol(const std::string& hostname, const std::string& servname):
        hostname(hostname), skt(hostname.c_str(), servname.c_str()) {}

// client
int BinaryProtocol::enter_lobby(const std::string username) {
    std::ostringstream request;
    char hello = HELLO_MSG;
    // request.put(HELLO_MSG);
    // request.write(username.c_str(), username.size());
    // request << 0x01 << htons(username.size()) << username;
    request.write(&hello, 1);

    uint16_t username_len = htons(username.size());
    request.write(reinterpret_cast<const char*>(&username_len), sizeof(username_len));

    request << username;


    auto buf = request.str();
    skt.sendall(buf.data(), buf.size());

    uint8_t response_buf[PROTOCOL_MSG_SIZE];
    skt.recvall(response_buf, PROTOCOL_MSG_SIZE);

    return response_buf[1];
}

// server
BinaryProtocol::BinaryProtocol(const std::string& servname): skt(servname.c_str()) {}

std::string BinaryProtocol::wait_for_player() {
    Socket peer_skt = skt.accept();

    uint8_t buf[HELLO_MSG_SIZE];
    peer_skt.recvall(buf, HELLO_MSG_SIZE);
    uint16_t username_len = ntohs(*(uint16_t*)(buf + 1));
    // char username[username_len];
    std::string username(username_len, '\0');
    peer_skt.recvall(username.data(), username_len);

    std::ostringstream response;
    char p_msg = PROTOCOL_MSG;
    response.write(&p_msg, 1);
    char bin_p = BIN_PROTOCOL;
    response.write(&bin_p, 1);
    // response << PROTOCOL_MSG << BIN_PROTOCOL;
    auto response_buf = response.str();
    peer_skt.sendall(response_buf.data(), response_buf.size());

    return username;
}
