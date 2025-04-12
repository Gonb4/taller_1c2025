#ifndef SETUP_PROTOCOL_H
#define SETUP_PROTOCOL_H

#include <string>
#include <memory>

#include "socket.h"
#include "bin_protocol.h"
#include "txt_protocol.h"

class SetupProtocol {
private:
    Socket skt;

public:
    // client
    SetupProtocol(const std::string& hostname, const std::string& servname);

    std::unique_ptr<Protocol> request_login(const std::string& username);


    // server
    SetupProtocol(const std::string& servname);

    std::pair<std::unique_ptr<Protocol>, std::string> wait_for_player(const uint8_t p_type);
    // std::string receive_username();


    SetupProtocol(const SetupProtocol&) = delete;
    SetupProtocol& operator=(const SetupProtocol&) = delete;

    SetupProtocol(SetupProtocol&&) = default;
    SetupProtocol& operator=(SetupProtocol&&) = default;

    ~SetupProtocol() = default;
};

#endif
