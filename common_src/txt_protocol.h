#ifndef TXT_PROTOCOL_H
#define TXT_PROTOCOL_H

#include <string>

#include "socket.h"
#include "protocol.h"
#include "constants.h"
// #include <sstream>

class TextProtocol : public Protocol {
private:
    //client
    void request_weapon_purchase(const Transaction& t);
    void request_ammo_purchase(const Transaction& t);

    //server
    Transaction await_weapon_purchase();
    Transaction await_ammo_purchase();

public:
    TextProtocol(Socket&& s);
    bool disconnected() override;

    // client
    PlayerInventory await_inventory_update() override;
    void request_transaction(const Transaction& t) override;
    
    // server
    void send_inventory(const PlayerInventory&) override;
    Transaction await_transaction() override;


    TextProtocol(const TextProtocol&) = delete;
    TextProtocol& operator=(const TextProtocol&) = delete;

    TextProtocol(TextProtocol&&) = default;
    TextProtocol& operator=(TextProtocol&&) = default;

    ~TextProtocol() = default;
};

#endif