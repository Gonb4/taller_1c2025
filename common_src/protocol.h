#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "socket.h"
#include "constants.h"
// #include <sstream>

class Protocol {
protected:
    Socket skt;

    Protocol(Socket&& s) : skt(std::move(s)) {}

public:
    // client
    virtual PlayerInventory await_inventory_update() = 0;
    //request_transaction
    //request_weapon_purchase (agregar constante "buy")
    //request_ammo_purchase (agregar constante "ammo")
    
    // server
    virtual bool player_disconnected() = 0;
    virtual void send_inventory(const PlayerInventory&) = 0;
    //await_transaction (devuelve Transaction, llama a las de abajo)
    //-await_weapon_purchase (privado?)
    //-await_ammo_purchase (privado?)

    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;

    virtual ~Protocol() = default;
};

#endif