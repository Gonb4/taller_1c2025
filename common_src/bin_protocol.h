#ifndef BIN_PROTOCOL_H
#define BIN_PROTOCOL_H

#include <string>

#include "socket.h"
// #include <sstream>

class BinaryProtocol {
private:
    const std::string hostname;
    const std::string servname;
    Socket skt;

public:
    BinaryProtocol(const std::string& hostname, const std::string& servname);

    explicit BinaryProtocol(const std::string& servname);

    // void async_get(const std::string& resource);
    // std::string wait_response(bool include_headers=false);
    // std::string get(const std::string& resource, bool include_headers=false);

    // std::string weapon_code_to_name(int code)
    // int weapon_name_to_code(std::string name)
    //  switch

    // client
    int enter_lobby(const std::string username);

    // server
    std::string wait_for_player();

    BinaryProtocol(const BinaryProtocol&) = delete;
    BinaryProtocol& operator=(const BinaryProtocol&) = delete;

    BinaryProtocol(BinaryProtocol&&) = default;
    BinaryProtocol& operator=(BinaryProtocol&&) = default;
};

#endif
