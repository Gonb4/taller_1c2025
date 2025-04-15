#ifndef TXT_PROTOCOL_H
#define TXT_PROTOCOL_H

#include "socket.h"
#include "protocol.h"
#include "constants.h"

class TextProtocol : public Protocol {
private:
    std::istringstream receive_message(int n);

    //client
    void request_weapon_purchase(const Transaction& t);
    void request_ammo_purchase(const Transaction& t);

    //server
    std::pair<bool, Transaction> await_weapon_purchase(const std::string& line);
    std::pair<bool, Transaction> await_ammo_purchase(const std::string& line);

public:
    TextProtocol(Socket&& s);
    bool disconnected() override;

    // client
    PlayerInventory await_inventory_update() override;
    void request_transaction(const Transaction& t) override;
    
    // server
    void send_inventory(const PlayerInventory& p_inv) override;
    std::pair<bool, Transaction> await_transaction() override;


    TextProtocol(const TextProtocol&) = delete;
    TextProtocol& operator=(const TextProtocol&) = delete;

    TextProtocol(TextProtocol&&) = default;
    TextProtocol& operator=(TextProtocol&&) = default;

    ~TextProtocol() = default;
};

#endif