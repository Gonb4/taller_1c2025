#ifndef BIN_PROTOCOL_H
#define BIN_PROTOCOL_H

// #include <string>

#include "socket.h"
#include "weapon_encoder.h"
#include "constants.h"
#include "protocol.h"
// #include <sstream>

class BinaryProtocol : public Protocol {
private:
    WeaponEncoder wpn_encoder;

public:
    BinaryProtocol(Socket&& s);

    // client
    PlayerInventory await_inventory_update() override;
    //request_transaction
    //request_weapon_purchase (agregar constante "buy")
    //request_ammo_purchase (agregar constante "ammo")
    
    // server
    bool player_disconnected() override;
    void send_inventory(const PlayerInventory&) override;
    //await_transaction (devuelve Transaction, llama a las de abajo)
    //-await_weapon_purchase (privado?)
    //-await_ammo_purchase (privado?)

    BinaryProtocol(const BinaryProtocol&) = delete;
    BinaryProtocol& operator=(const BinaryProtocol&) = delete;

    BinaryProtocol(BinaryProtocol&&) = default;
    BinaryProtocol& operator=(BinaryProtocol&&) = default;

    ~BinaryProtocol() = default;
};

#endif


    // void async_get(const std::string& resource);
    // std::string wait_response(bool include_headers=false);
    // std::string get(const std::string& resource, bool include_headers=false);