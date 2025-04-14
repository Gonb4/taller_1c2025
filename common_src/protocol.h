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
    virtual bool disconnected() = 0;

    // client
    virtual PlayerInventory await_inventory_update() = 0;
    virtual void request_transaction(const Transaction& t) = 0;
    
    // server
    virtual void send_inventory(const PlayerInventory&) = 0;
    virtual Transaction await_transaction() = 0;


    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;

    virtual ~Protocol() = default;
};

#endif