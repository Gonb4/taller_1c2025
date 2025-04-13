#ifndef TXT_PROTOCOL_H
#define TXT_PROTOCOL_H

#include <string>

#include "socket.h"
#include "protocol.h"
#include "constants.h"
// #include <sstream>

class TextProtocol : public Protocol {
private:

public:
    TextProtocol(Socket&& s);

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

    TextProtocol(const TextProtocol&) = delete;
    TextProtocol& operator=(const TextProtocol&) = delete;

    TextProtocol(TextProtocol&&) = default;
    TextProtocol& operator=(TextProtocol&&) = default;

    ~TextProtocol() = default;
};

#endif