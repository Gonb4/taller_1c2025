#ifndef BIN_PROTOCOL_H
#define BIN_PROTOCOL_H

#include <string>

#include "socket.h"
#include "weapon_encoder.h"
#include "constants.h"
// #include <sstream>

class BinaryProtocol {
private:
    // const std::string hostname;
    // const std::string servname;
    Socket peer_skt;
    Socket skt;
    WeaponEncoder wpn_encoder;

public:
    // client
    BinaryProtocol(const std::string& hostname, const std::string& servname);

    uint8_t enter_lobby(const std::string& username);
    PlayerInventory await_inventory_update();
    //request_transaction
    
    // server
    explicit BinaryProtocol(const std::string& servname);

    std::string wait_for_player();
    void send_inventory(const PlayerInventory&);
    //await_transaction

    BinaryProtocol(const BinaryProtocol&) = delete;
    BinaryProtocol& operator=(const BinaryProtocol&) = delete;

    BinaryProtocol(BinaryProtocol&&) = default;
    BinaryProtocol& operator=(BinaryProtocol&&) = default;
};

#endif


    // void async_get(const std::string& resource);
    // std::string wait_response(bool include_headers=false);
    // std::string get(const std::string& resource, bool include_headers=false);