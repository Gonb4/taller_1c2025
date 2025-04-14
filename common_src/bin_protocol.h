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

    //client
    void request_weapon_purchase(const Transaction& t);
    void request_ammo_purchase(const Transaction& t);

    //server
    Transaction await_weapon_purchase();
    Transaction await_ammo_purchase();

public:
    BinaryProtocol(Socket&& s);
    bool disconnected() override;

    // client
    PlayerInventory await_inventory_update() override;
    void request_transaction(const Transaction& t) override;

    // server
    void send_inventory(const PlayerInventory&) override;
    Transaction await_transaction() override;


    BinaryProtocol(const BinaryProtocol&) = delete;
    BinaryProtocol& operator=(const BinaryProtocol&) = delete;

    BinaryProtocol(BinaryProtocol&&) = default;
    BinaryProtocol& operator=(BinaryProtocol&&) = default;

    ~BinaryProtocol() = default;
};

#endif
