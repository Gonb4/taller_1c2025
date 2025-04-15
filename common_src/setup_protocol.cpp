#include "setup_protocol.h"

#include <sstream>
#include <utility>

#include <arpa/inet.h>

#include "constants.h"
// #include "liberror.h"


// client
SetupProtocol::SetupProtocol(const std::string& hostname, const std::string& servname):
        skt(hostname.c_str(), servname.c_str()) {}


std::unique_ptr<Protocol> SetupProtocol::request_login(const std::string& username) {
    std::ostringstream request;
    request.put(LOGIN_MSG);

    uint16_t username_len = htons(username.size());
    request.write(reinterpret_cast<const char*>(&username_len), sizeof(username_len));

    request << username;

    auto buf = request.str();
    skt.sendall(buf.data(), buf.size());

    uint8_t response_buf[PROTOCOL_MSG_SIZE];
    skt.recvall(response_buf, PROTOCOL_MSG_SIZE);

    if (response_buf[1] == BIN_PROTOCOL)
        return std::make_unique<BinaryProtocol>(std::move(skt));
    else
        return std::make_unique<TextProtocol>(std::move(skt));
}

// server
SetupProtocol::SetupProtocol(const std::string& servname): skt(servname.c_str()) {}


std::pair<std::unique_ptr<Protocol>, std::string> SetupProtocol::wait_for_player(
        const uint8_t p_type) {
    Socket peer_skt = skt.accept();

    uint8_t buf[LOGIN_MSG_SIZE];
    peer_skt.recvall(buf, LOGIN_MSG_SIZE);

    uint16_t username_len = ntohs(*(uint16_t*)(buf + 1));
    std::string username(username_len, '\0');
    peer_skt.recvall(username.data(), username_len);

    std::ostringstream info;
    info.put(PROTOCOL_MSG);
    info.put(p_type);

    auto info_buf = info.str();
    peer_skt.sendall(info_buf.data(), info_buf.size());

    std::unique_ptr<Protocol> protocol;

    if (p_type == BIN_PROTOCOL)
        protocol = std::make_unique<BinaryProtocol>(std::move(peer_skt));
    else
        protocol = std::make_unique<TextProtocol>(std::move(peer_skt));


    return {std::move(protocol), std::move(username)};
    // throw LibError(errno, "invalid protocol type: %s", p_type);
}

// std::string SetupProtocol::receive_username() {
//     return username;
// }
